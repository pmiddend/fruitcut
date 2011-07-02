#include "menu.hpp"
#include "../config_variables.hpp"
#include "ingame/running.hpp"
#include "../events/generic_transition.hpp"
#include "../../fruitlib/json/find_and_convert_member.hpp"
#include "../../fruitlib/resource_tree/path.hpp"
#include "../../fruitlib/json/modify_user_value.hpp"
#include "../../fruitlib/audio/sound_controller.hpp"
#include "../../fruitlib/audio/music_controller.hpp"
#include "../../media_path.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/state/state.hpp>
#include <sge/renderer/viewport_size.hpp>
#include <sge/systems/instance.hpp>
#include <sge/viewport/manager.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/string.hpp>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUISlider.h>

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
					std::tr1::placeholders::_1)))),
	effects_slider_pulled_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("MainMenu/EffectsVolume")->subscribeEvent(
			CEGUI::Slider::EventValueChanged,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&menu::effects_slider_pulled,
					this,
					std::tr1::placeholders::_1)))),
	splatter_slider_pulled_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("MainMenu/SplatterFactor")->subscribeEvent(
			CEGUI::Slider::EventValueChanged,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&menu::splatter_slider_pulled,
					this,
					std::tr1::placeholders::_1))))
{
	dynamic_cast<CEGUI::Slider *>(
		CEGUI::WindowManager::getSingleton().getWindow("MainMenu/MusicVolume"))->setCurrentValue(
		static_cast<float>(
			context<machine>().config_variables().music_volume().value()));

	dynamic_cast<CEGUI::Slider *>(
		CEGUI::WindowManager::getSingleton().getWindow("MainMenu/EffectsVolume"))->setCurrentValue(
		static_cast<float>(
			context<machine>().config_variables().effects_volume().value()));

	dynamic_cast<CEGUI::Slider *>(
		CEGUI::WindowManager::getSingleton().getWindow("MainMenu/SplatterFactor"))->setCurrentValue(
		static_cast<float>(
			context<machine>().config_variables().splatter_count_to_area_factor().value()));

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
	context<machine>().config_variables().music_volume().value(
		static_cast<sge::audio::scalar>(
			dynamic_cast<CEGUI::Slider *>(
				CEGUI::WindowManager::getSingleton().getWindow("MainMenu/MusicVolume"))->getCurrentValue()));
	return true;
}

bool
fruitcut::app::states::menu::effects_slider_pulled(
	CEGUI::EventArgs const &)
{
	context<machine>().config_variables().effects_volume().value(
		static_cast<sge::audio::scalar>(
			dynamic_cast<CEGUI::Slider *>(
				CEGUI::WindowManager::getSingleton().getWindow("MainMenu/EffectsVolume"))->getCurrentValue()));
	return true;
}

bool
fruitcut::app::states::menu::splatter_slider_pulled(
	CEGUI::EventArgs const &)
{
	context<machine>().config_variables().splatter_count_to_area_factor().value(
		static_cast<fruit::area::value_type>(
			dynamic_cast<CEGUI::Slider *>(
				CEGUI::WindowManager::getSingleton().getWindow("MainMenu/SplatterFactor"))->getCurrentValue()));
	return true;
}
