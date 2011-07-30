#include "ranking.hpp"
#include "../menu/main.hpp"
#include "../menu/highscore.hpp"
#include "../../events/define_transition_reaction.hpp"
#include "../../depths/root.hpp"
#include "../../events/return_post_transition_functor.hpp"
#include "../../../media_path.hpp"
#include "../../../fruitlib/scenic/parent.hpp"
#include "../../../fruitlib/scenic/events/update.hpp"
#include "../../highscore/providers_from_json.hpp"
#include "../../highscore/name.hpp"
#include "../../highscore/score.hpp"
#include <sge/systems/instance.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/from_cegui_string.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <CEGUIWindowManager.h>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>

fruitcut::app::states::gameover::ranking::ranking(
	my_context ctx)
:
	my_base(
		ctx),
	node_base(
		fruitlib::scenic::parent(
			context<app::machine>().root_node(),
			fruitlib::scenic::depth(
				depths::root::dont_care))),
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
			"Ranking/Highscores")),
	quit_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Ranking/Quit")),
	main_menu_button_connection_(
		main_menu_button_.push_callback(
			FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main))),
	highscore_button_connection_(
		highscore_button_.push_callback(
			FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::highscore))),
	quit_button_connection_(
		quit_button_.push_callback(
			std::tr1::bind(
				&app::machine::quit,
				&context<app::machine>()))),
	providers_(),
	post_model_(
		providers_),
	table_view_(
		context<app::machine>().systems().charconv_system(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Ranking/List"),
		post_model_),
	message_received_connection_(
		post_model_.message_received(
			std::tr1::bind(
				&ranking::message_received,
				this,
				std::tr1::placeholders::_1))),
	error_received_connection_(
		post_model_.error_received(
			std::tr1::bind(
				&ranking::error_received,
				this,
				std::tr1::placeholders::_1)))
{
	app::highscore::providers_from_json(
		context<app::machine>().config_file(),
		providers_);

	post_model_.post(
		highscore::name(
			sge::cegui::from_cegui_string(
				context<superstate>().name(),
				context<machine>().systems().charconv_system())),
		highscore::score(
			context<machine>().last_game_score()));
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
fruitcut::app::states::gameover::ranking::react(
	fruitlib::scenic::events::update const &)
{
	post_model_.update();
}

void
fruitcut::app::states::gameover::ranking::message_received(
	fcppt::string const &s)
{
	CEGUI::Window &w = 
		*context<app::machine>().gui_system().window_manager().getWindow(
			"Ranking/MessageLog");

	w.setText(
		w.getText()+
		sge::cegui::to_cegui_string(
			s,
			context<app::machine>().systems().charconv_system()));
}

void
fruitcut::app::states::gameover::ranking::error_received(
	fcppt::string const &s)
{
	CEGUI::Window &w = 
		*context<app::machine>().gui_system().window_manager().getWindow(
			"Ranking/MessageLog");

	w.setText(
		w.getText()+
		sge::cegui::to_cegui_string(
			s,
			context<app::machine>().systems().charconv_system()));
}
