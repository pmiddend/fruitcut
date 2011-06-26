#include "menu.hpp"
#include "ingame/running.hpp"
#include "../events/generic_transition.hpp"
#include "../../fruitlib/time_format/string_to_duration_exn.hpp"
#include "../../fruitlib/json/find_and_convert_member.hpp"
#include "../../fruitlib/resource_tree/path.hpp"
#include "../../fruitlib/json/modify_user_value.hpp"
#include "../../fruitlib/audio/sound_controller.hpp"
#include "../../fruitlib/audio/music_controller.hpp"
#include "../../media_path.hpp"
#include "../postprocessing.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/state/state.hpp>
#include <sge/renderer/viewport_size.hpp>
#include <sge/systems/instance.hpp>
#include <sge/time/time.hpp>
#include <sge/viewport/manager.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/string.hpp>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUISlider.h>

namespace
{
class slider_wrapper
{
FCPPT_NONCOPYABLE(
	slider_wrapper);
public:
	typedef
	fcppt::function::object<void(float)>
	slid_callback;

	explicit
	slider_wrapper(
		CEGUI::Window &_window,
		float const _initial_value,
		sge::parse::json::object &_global_config,
		sge::parse::json::object &_user_config,
		fruitcut::fruitlib::json::path const &_path,
		slid_callback const &_callback)
	:
		window_(
			dynamic_cast<CEGUI::Slider &>(
				_window)),
		slid_connection_(
			window_.subscribeEvent(
			CEGUI::Slider::EventValueChanged,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&slider_wrapper::slid,
					this,
					std::tr1::placeholders::_1)))),
		global_config_(
			_global_config),
		user_config_(
			_user_config),
		path_(
			_path),
		callback_(
			_callback)
	{
		window_.setCurrentValue(
			_initial_value);
	}
private:
	CEGUI::Slider &window_;
	CEGUI::Event::ScopedConnection slid_connection_;
	sge::parse::json::object &global_config_;
	sge::parse::json::object &user_config_;
	fruitcut::fruitlib::json::path const path_;
	slid_callback callback_;

	bool
	slid(
		CEGUI::EventArgs const &)
	{
		fruitcut::fruitlib::json::modify_user_value(
			global_config_,
			user_config_,
			path_,
			static_cast<sge::parse::json::float_type>(
				window_.getCurrentValue()));
		
		callback_(
			window_.getCurrentValue());

		return true;
	}
};
}

fruitcut::app::states::menu::menu(
	my_context ctx)
:
	my_base(
		ctx),
	fruitlib::scenic::nodes::intrusive(),
	scoped_render_state_(
		context<machine>().systems().renderer(),
		sge::renderer::state::list
			(sge::renderer::state::depth_func::less)
			(sge::renderer::state::cull_mode::off)
			(sge::renderer::state::bool_::clear_depth_buffer = true)
			(sge::renderer::state::float_::depth_buffer_clear_val = 1.0f)),
	// Those timers will be activated as soon as we have a viewport
	saturation_timer_(
		fruitlib::time_format::string_to_duration_exn<sge::time::duration>(
			fruitlib::json::find_and_convert_member<fcppt::string>(
				context<machine>().config_file(),
				fruitlib::json::path(
					FCPPT_TEXT("menu"))
					/ FCPPT_TEXT("desaturation-time"))),
		sge::time::activation_state::inactive,
		context<machine>().timer_callback()),
	viewport_change_connection_(
		context<machine>().systems().viewport_manager().manage_callback(
			std::tr1::bind(
				&menu::viewport_change,
				this))),
	logo_(
		context<machine>().systems().renderer(),
		context<machine>().systems().viewport_manager(),
		context<machine>().systems().image_loader(),
		context<machine>().config_file()),
	gui_node_(
		context<machine>().gui_system(),
		context<machine>().timer_callback()),
	gui_keyboard_(
		context<machine>().gui_syringe(),
		context<machine>().systems().keyboard_collector()),
	gui_cursor_(
		context<machine>().gui_syringe(),
		context<machine>().systems().cursor_demuxer()),
	layout_(
		fruitcut::media_path()
			/FCPPT_TEXT("gui")
			/FCPPT_TEXT("layouts")
			/FCPPT_TEXT("main_menu.layout"),
		context<machine>().systems().charconv_system()),
	gui_sheet_(
		*CEGUI::WindowManager::getSingleton().getWindow("MainMenu")),
	quit_button_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("MainMenu/QuitButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&menu::quit_button_pushed,
					this,
					std::tr1::placeholders::_1)))),
	start_button_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("MainMenu/StartButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&menu::start_button_pushed,
					this,
					std::tr1::placeholders::_1)))),
	music_slider_pulled_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("MainMenu/MusicVolume")->subscribeEvent(
			CEGUI::Slider::EventValueChanged,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&menu::music_slider_pulled,
					this,
					std::tr1::placeholders::_1))))
{
	dynamic_cast<CEGUI::Slider *>(
		CEGUI::WindowManager::getSingleton().getWindow("MainMenu/MusicVolume"))->setCurrentValue(
		static_cast<float>(
			context<machine>().music_controller().volume()));

	context<machine>().music_controller().play(
		fruitlib::resource_tree::path(
			FCPPT_TEXT("menu")));

	// We already have a viewport? Ok, then go
	if(sge::renderer::viewport_size(context<machine>().systems().renderer()).content())
		viewport_change();

	context<machine>().root_node().insert_dont_care(
		*this);
	context<machine>().overlay_node().insert_dont_care(
		logo_);
	context<machine>().overlay_node().insert_dont_care(
		gui_node_);
}

void
fruitcut::app::states::menu::update()
{
	context<machine>().postprocessing().desaturate_filter().factor(
		static_cast<sge::renderer::scalar>(
			saturation_timer_.expired()
			?
				static_cast<sge::renderer::scalar>(1.0)
			:
				saturation_timer_.elapsed_frames()));
}

void
fruitcut::app::states::menu::render()
{
}

fruitcut::app::states::menu::~menu()
{
}

void
fruitcut::app::states::menu::viewport_change()
{
	if (saturation_timer_.active())
		return;

	saturation_timer_.activate();
}

bool
fruitcut::app::states::menu::quit_button_pushed(
	CEGUI::EventArgs const &)
{
	context<machine>().quit();
	return true;
}

bool
fruitcut::app::states::menu::start_button_pushed(
	CEGUI::EventArgs const &)
{
	context<machine>().post_event(
		events::generic_transition<ingame::running>());
	return true;
}

bool
fruitcut::app::states::menu::music_slider_pulled(
	CEGUI::EventArgs const &)
{
	context<machine>().music_controller().volume(
		static_cast<sge::audio::scalar>(
			dynamic_cast<CEGUI::Slider *>(
				CEGUI::WindowManager::getSingleton().getWindow("MainMenu/MusicVolume"))->getCurrentValue()));
	std::cout << "Changing music volume to " << context<machine>().music_controller().volume() << "...\n";
	return true;
}
