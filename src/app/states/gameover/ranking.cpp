#include "ranking.hpp"
#include "../menu/main.hpp"
#include "../menu/highscore.hpp"
#include "../../events/define_transition_reaction.hpp"
#include "../../events/return_post_transition_functor.hpp"
#include "../../../media_path.hpp"
#include "../../highscore/providers_from_json.hpp"
#include <sge/systems/instance.hpp>
#include <sge/cegui/system.hpp>
#include <CEGUIWindowManager.h>
#include <fcppt/text.hpp>

fruitcut::app::states::gameover::ranking::ranking(
	my_context ctx)
:
	my_base(
		ctx),
	fruitlib::scenic::nodes::intrusive(),
	layout_(
		context<machine>().gui_system(),
		fruitcut::media_path()
			/FCPPT_TEXT("gui")
			/FCPPT_TEXT("layouts")
			/FCPPT_TEXT("ranking.layout"),
		context<machine>().systems().charconv_system()),
	gui_sheet_(
		context<machine>().gui_system(),
		*context<machine>().gui_system().window_manager().getWindow("Ranking")),
	main_menu_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Ranking/MainMenu")),
	highscore_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Ranking/Highscore")),
	quit_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Ranking/Quit")),
	main_menu_button_connection_(
		main_menu_button_.push_callback(
			FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main))),
	highscore_button_connection_(
		main_menu_button_.push_callback(
			FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::highscore))),
	quit_button_connection_(
		quit_button_.push_callback(
			std::tr1::bind(
				&app::machine::quit,
				&context<app::machine>()))),
	providers_()/*,
	table_view_(
		context<app::machine>().systems().charconv_system(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Ranking/List"),
		table_model_)*/
{
	context<app::machine>().root_node().insert_dont_care(
		*this);

	app::highscore::providers_from_json(
		context<app::machine>().config_file(),
		providers_);
}

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::main,
	gameover::ranking)

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::highscore,
	gameover::ranking)

fruitcut::app::states::gameover::ranking::~ranking()
{
}

void
fruitcut::app::states::gameover::ranking::update()
{
}

void
fruitcut::app::states::gameover::ranking::render()
{
}
