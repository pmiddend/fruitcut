#include <fruitapp/states/ingame/paused.hpp>
#include <fruitapp/states/ingame/running.hpp>
#include <fruitapp/states/menu/main.hpp>
#include <fruitlib/time_format/find_and_convert_duration.hpp>
#include <fruitlib/json/find_and_convert_member.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <fruitlib/pp/texture/use_screen_size.hpp>
#include <fruitlib/pp/filter/blur.hpp>
#include "../../../media_path.hpp"
#include <fruitapp/postprocessing.hpp>
#include <fruitapp/depths/root.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/return_post_transition_functor.hpp>
#include <fruitapp/events/generic_transition.hpp>
#include <fruitapp/scene.hpp>
#include <sge/renderer/device.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/systems/instance.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <sge/cegui/system.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/assign/make_container.hpp>
#include <iostream>
#include <CEGUIWindowManager.h>

fruitapp::states::ingame::paused::paused(
	my_context ctx)
:
	my_base(
		ctx),
	node_base(
		fruitlib::scenic::parent(
			context<machine>().overlay_node(),
			fruitlib::scenic::depth(
				depths::overlay::paused))),
	time_factor_(
		context<machine>(),
		0.0f),
	scene_deactivation_(
		context<machine>().scene_node(),
		false),
	pp_deactivation_(
		context<machine>().postprocessing(),
		false),
	system_(
		context<machine>().postprocessing().filter_manager()),
	inject_texture_(
		context<machine>().postprocessing().texture_manager()),
	blur_(
		context<machine>().systems().renderer(),
		context<machine>().postprocessing().filter_manager(),
		context<machine>().postprocessing().texture_manager(),
		fruitlib::pp::texture::use_screen_size(),
		static_cast<fruitlib::pp::filter::blur::size_type>(
			1)),
	current_texture_(
		context<machine>().postprocessing().result_texture()),
	blur_iterations_(
		static_cast<fruitlib::pp::filter::blur::size_type>(
			0)),
	max_blur_iterations_(
		fruitlib::json::find_and_convert_member<fruitlib::pp::filter::blur::size_type>(
			context<machine>().config_file(),
			fruitlib::json::path(FCPPT_TEXT("paused"))
				/ FCPPT_TEXT("max-blur-iterations"))),
	blur_timer_(
		sge::timer::parameters<sge::timer::clocks::standard>(
			fruitlib::time_format::find_and_convert_duration<sge::timer::clocks::standard::duration>(
				context<machine>().config_file(),
				fruitlib::json::path(FCPPT_TEXT("paused"))
					/ FCPPT_TEXT("blur-frequency-time")))),
	transit_to_running_connection_(
		context<machine>().systems().keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape, 
				std::tr1::bind(
					// Note that using post_event does something unexpected. If
					// you use that, you get a tick event first and _then_ the
					// toggle_pause event, which is not the desired behaviour
					// (post_event posts to the queue, process_event immediately
					// processes it)
					&machine::post_event,
					&context<machine>(),
					events::generic_transition<ingame::running>())))),
	gui_node_(
		fruitlib::scenic::parent(
			context<machine>().overlay_node(),
			fruitlib::scenic::depth(
				depths::overlay::dont_care)),
		context<machine>().gui_system(),
		context<fruitapp::machine>().standard_clock_callback()),
	gui_keyboard_(
		context<machine>().gui_syringe(),
		context<machine>().systems().keyboard_collector()),
	gui_cursor_(
		context<machine>().gui_syringe(),
		context<machine>().systems().cursor_demuxer()),
	layout_(
		context<machine>().gui_system(),
		fruitcut::media_path()
			/FCPPT_TEXT("gui")
			/FCPPT_TEXT("layouts")
			/FCPPT_TEXT("ingame_menu.layout"),
		context<machine>().systems().charconv_system()),
	gui_sheet_(
		context<machine>().gui_system(),
		*context<machine>().gui_system().window_manager().getWindow("MainMenu")),
	continue_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"IngameMenu/Continue")),
	main_menu_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"IngameMenu/MainMenu")),
	quit_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"IngameMenu/Quit")),
	continue_connection_(
		continue_button_.push_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				ingame::running))),
	main_menu_connection_(
		main_menu_button_.push_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main))),
	quit_connection_(
		quit_button_.push_callback(
			std::tr1::bind(
				&fruitapp::machine::quit,
				&context<fruitapp::machine>())))
{
	system_.add_filter(
		inject_texture_,
		FCPPT_TEXT("inject_texture"),
		fruitlib::pp::dependency_set());

	system_.add_filter(
		blur_,
		FCPPT_TEXT("blur"),
		fcppt::assign::make_container<fruitlib::pp::dependency_set>
			(FCPPT_TEXT("inject_texture")));
}

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	ingame::running,
	ingame::paused)

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::main,
	ingame::paused)

fruitapp::states::ingame::paused::~paused()
{
}

void
fruitapp::states::ingame::paused::react(
	fruitlib::scenic::events::render const &)
{
	system_.render_result();
}

void
fruitapp::states::ingame::paused::react(
	fruitlib::scenic::events::update const &)
{
	if(!blur_iterations_ || (blur_iterations_ < max_blur_iterations_ && sge::timer::reset_when_expired(blur_timer_)))
	{
		inject_texture_.texture(
			current_texture_);
		system_.update();
		current_texture_ = 
			system_.result_texture();
		blur_iterations_++;
	}
}

