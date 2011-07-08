#include "highscore.hpp"
#include "main.hpp"
#include "../../events/define_transition_reaction.hpp"
#include "../../events/return_post_transition_functor.hpp"
#include "../../../media_path.hpp"
#include <sge/systems/instance.hpp>
#include <sge/cegui/system.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <CEGUIWindowManager.h>

fruitcut::app::states::menu::highscore::highscore(
	my_context ctx)
:
	my_base(
		ctx),
	layout_(
		context<machine>().gui_system(),
		fruitcut::media_path()
			/FCPPT_TEXT("gui")
			/FCPPT_TEXT("layouts")
			/FCPPT_TEXT("new_highscore.layout"),
		context<machine>().systems().charconv_system()),
	gui_sheet_(
		context<machine>().gui_system(),
		*context<machine>().gui_system().window_manager().getWindow("Highscores")),
	main_menu_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Highscores/MainMenu")),
	quit_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Highscores/Quit")),
	main_menu_button_connection_(
		main_menu_button_.push_callback(
			FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main))),
	quit_button_connection_(
		quit_button_.push_callback(
			std::tr1::bind(
				&app::machine::quit,
				&context<app::machine>())))
{
}

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::main,
	menu::highscore)

fruitcut::app::states::menu::highscore::~highscore()
{
}
