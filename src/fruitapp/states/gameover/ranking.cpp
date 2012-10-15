#include <fruitapp/depths/root.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/return_post_transition_functor.hpp>
#include <fruitapp/highscore/name.hpp>
#include <fruitapp/highscore/providers_from_json.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fruitapp/states/gameover/ranking.hpp>
#include <fruitapp/states/menu/highscore.hpp>
#include <fruitapp/states/menu/main.hpp>
#include <fruitlib/media_path.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <sge/cegui/from_cegui_string.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <sge/systems/instance.hpp>
#include <awl/main/exit_success.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <fcppt/config/external_end.hpp>


fruitapp::states::gameover::ranking::ranking(
	my_context ctx)
:
	my_base(
		ctx),
	node_base(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care)))),
	layout_(
		context<machine>().gui_system(),
		fruitlib::media_path()
			/FCPPT_TEXT("gui")
			/FCPPT_TEXT("layouts")
			/FCPPT_TEXT("ranking.layout")),
	gui_sheet_(
		context<machine>().gui_system(),
		*layout_.window().getChild(
			"Ranking")),
	main_menu_button_(
		context<machine>().sound_controller(),
		*layout_.window().getChild(
			"Ranking/MainMenu")),
	highscore_button_(
		context<machine>().sound_controller(),
		*layout_.window().getChild(
			"Ranking/Highscores")),
	quit_button_(
		context<machine>().sound_controller(),
		*layout_.window().getChild(
			"Ranking/Quit")),
	main_menu_button_connection_(
		main_menu_button_.push_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main))),
	highscore_button_connection_(
		highscore_button_.push_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::highscore))),
	quit_button_connection_(
		quit_button_.push_callback(
			std::tr1::bind(
				&fruitapp::machine::quit,
				&context<fruitapp::machine>(),
				awl::main::exit_success()))),
	providers_(),
	post_model_(
		providers_),
	table_view_(
		context<fruitapp::machine>().systems().charconv_system(),
		*layout_.window().getChild(
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
	fruitapp::highscore::providers_from_json(
		context<fruitapp::machine>().systems().charconv_system(),
		context<fruitapp::machine>().config_file(),
		providers_);

	post_model_.post(
		highscore::name(
			sge::cegui::from_cegui_string(
				context<superstate>().name(),
				context<machine>().systems().charconv_system())),
		highscore::score(
			context<machine>().last_game_score()));
}

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::main,
	gameover::ranking)

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::highscore,
	gameover::ranking)

fruitapp::states::gameover::ranking::~ranking()
{
}

void
fruitapp::states::gameover::ranking::react(
	fruitlib::scenic::events::update const &)
{
	post_model_.update();
}

void
fruitapp::states::gameover::ranking::message_received(
	fcppt::string const &s)
{
	CEGUI::Window &w =
		*layout_.window().getChild(
			"Ranking/MessageLog");

	w.setText(
		w.getText()+
		sge::cegui::to_cegui_string(
			s,
			context<fruitapp::machine>().systems().charconv_system()));
}

void
fruitapp::states::gameover::ranking::error_received(
	fcppt::string const &s)
{
	CEGUI::Window &w =
		*layout_.window().getChild(
			"Ranking/MessageLog");

	w.setText(
		w.getText()+
		sge::cegui::to_cegui_string(
			s,
			context<fruitapp::machine>().systems().charconv_system()));
}
