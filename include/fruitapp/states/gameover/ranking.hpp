#ifndef FRUITAPP_STATES_GAMEOVER_RANKING_HPP_INCLUDED
#define FRUITAPP_STATES_GAMEOVER_RANKING_HPP_INCLUDED

#include <fruitapp/events/declare_transition_reaction.hpp>
#include <fruitapp/events/declare_transition_type.hpp>
#include <fruitapp/gui/dialogs/ranking_unique_ptr.hpp>
#include <fruitapp/states/gameover/superstate.hpp>
#include <fruitapp/states/menu/highscore_fwd.hpp>
#include <fruitapp/states/menu/main_fwd.hpp>
#include <fruitapp/highscore/provider_sequence.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fcppt/string.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/statechart/state.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace states
{
namespace gameover
{
class ranking
:
	// The second argument has to be a complete type
	public boost::statechart::state<ranking,superstate>,
	public fruitlib::scenic::node<ranking>
{
FCPPT_NONCOPYABLE(
	ranking);
public:
	typedef
	boost::mpl::vector2
	<
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::main),
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::highscore)
	>
	reactions;

	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	ranking(
		my_context);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::main);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::highscore);

	~ranking();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	fruitapp::highscore::provider_sequence providers_;
	fruitapp::gui::dialogs::ranking_unique_ptr ranking_;
	fcppt::signal::scoped_connection main_menu_button_connection_;
	fcppt::signal::scoped_connection highscore_button_connection_;
	fcppt::signal::scoped_connection quit_button_connection_;
	/*
	highscore::post_model post_model_;
	gui::table::view table_view_;
	fcppt::signal::scoped_connection message_received_connection_;
	fcppt::signal::scoped_connection error_received_connection_;

	void
	message_received(
		fcppt::string const &);

	void
	error_received(
		fcppt::string const &);
	*/
};
}
}
}

#endif
