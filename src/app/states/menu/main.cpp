#include "main.hpp"
#include "settings.hpp"
#include "highscore.hpp"
#include "../ingame/running.hpp"
#include "../../events/define_transition_reaction.hpp"
#include "../../events/return_post_transition_functor.hpp"
#include "../../../fruitlib/scenic/parent.hpp"
#include "../../../fruitlib/scenic/depth.hpp"
#include "../../../media_path.hpp"
#include "../../depths/overlay.hpp"
#include <sge/systems/instance.hpp>
#include <sge/cegui/system.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <CEGUIWindowManager.h>

fruitcut::app::states::menu::main::main(
	my_context ctx)
:
	my_base(
		ctx),
	logo_(
		fruitlib::scenic::parent(
			context<app::machine>().overlay_node(),
			fruitlib::scenic::depth(
				depths::overlay::dont_care)),
		context<machine>().systems().renderer(),
		context<machine>().systems().viewport_manager(),
		context<machine>().systems().image_loader(),
		context<machine>().config_file()),
	layout_(
		context<machine>().gui_system(),
		fruitcut::media_path()
			/FCPPT_TEXT("gui")
			/FCPPT_TEXT("layouts")
			/FCPPT_TEXT("main_menu.layout"),
		context<machine>().systems().charconv_system()),
	gui_sheet_(
		context<machine>().gui_system(),
		*context<machine>().gui_system().window_manager().getWindow("MainMenu")),
	settings_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"MainMenu/Settings")),
	highscore_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"MainMenu/Highscores")),
	quit_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"MainMenu/Quit")),
	start_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"MainMenu/StartGame")),
	settings_button_connection_(
		settings_button_.push_callback(
			FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::settings))),
	highscore_button_connection_(
		highscore_button_.push_callback(
			FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::highscore))),
	quit_button_connection_(
		quit_button_.push_callback(
			std::tr1::bind(
				&app::machine::quit,
				&context<app::machine>()))),
	start_button_connection_(
		start_button_.push_callback(
			FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				ingame::superstate)))
{
}

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	ingame::superstate,
	menu::main)

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::settings,
	menu::main)

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::highscore,
	menu::main)

fruitcut::app::states::menu::main::~main()
{
}
