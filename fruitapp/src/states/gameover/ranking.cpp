#include <fruitapp/depths/root.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/return_post_transition_functor.hpp>
#include <fruitapp/gui/system.hpp>
#include <fruitapp/gui/dialogs/ranking.hpp>
#include <fruitapp/highscore/name.hpp>
#include <fruitapp/highscore/providers_from_json.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fruitapp/states/gameover/ranking.hpp>
#include <fruitapp/states/menu/highscore.hpp>
#include <fruitapp/states/menu/main.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <sge/cegui/from_cegui_string.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <sge/systems/instance.hpp>
#include <awl/main/exit_success.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>


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
	providers_(),
	ranking_(
		context<fruitapp::machine>().gui_system().create_ranking(
			providers_)),
	main_menu_button_connection_(
		ranking_->register_main_menu_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main))),
	highscore_button_connection_(
		ranking_->register_highscore_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::highscore))),
	quit_button_connection_(
		ranking_->register_quit_callback(
			std::tr1::bind(
				&fruitapp::machine::quit,
				&context<fruitapp::machine>(),
				awl::main::exit_success())))
{
	fruitapp::highscore::providers_from_json(
		context<fruitapp::machine>().systems().charconv_system(),
		context<fruitapp::machine>().config_file(),
		providers_);

	ranking_->post(
		fruitapp::highscore::name(
			context<fruitapp::states::gameover::superstate>().name()),
		fruitapp::highscore::score(
			context<fruitapp::machine>().last_game_score()));
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
	ranking_->update();
}
