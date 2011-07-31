#ifndef FRUITAPP_STATES_INGAME_RUNNING_HPP_INCLUDED
#define FRUITAPP_STATES_INGAME_RUNNING_HPP_INCLUDED

#include "superstate.hpp"
#include "paused_fwd.hpp"
#include <fruitapp/sword_trail.hpp>
#include <fruitapp/cursor_trail.hpp>
#include <fruitapp/states/gameover/superstate_fwd.hpp>
#include <fruitapp/fruit/object_fwd.hpp>
#include <fruitapp/events/declare_transition_type.hpp>
#include <fruitapp/events/declare_transition_reaction.hpp>
#include <fruitlib/scenic/adaptors/line_drawer.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/events/update_fwd.hpp>
#include <fruitlib/scenic/events/viewport_change_fwd.hpp>
#include <sge/line_drawer/object.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/line_drawer/line_sequence.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/statechart/state.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitapp
{
namespace states
{
namespace ingame
{
class running
:
	// The second argument has to be a complete type
	public boost::statechart::state<running,superstate>,
	public fruitlib::scenic::node<running>
{
FCPPT_NONCOPYABLE(
	running);
public:
	typedef
	boost::mpl::vector2
	<
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			ingame::paused),
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			gameover::superstate)
	>
	reactions;

	typedef
	boost::mpl::vector2
	<
		fruitlib::scenic::events::update,
		fruitlib::scenic::events::viewport_change
	>
	scene_reactions;

	explicit
	running(
		my_context);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		ingame::paused);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		gameover::superstate);

	~running();

	void
	react(
		fruitlib::scenic::events::update const &);

	void
	react(
		fruitlib::scenic::events::viewport_change const &);
private:
	sge::line_drawer::object line_drawer_;
	fruitlib::scenic::adaptors::line_drawer line_drawer_node_;
	fruitapp::cursor_trail cursor_trail_;
	fcppt::signal::scoped_connection fruit_spawned_connection_;
	bool draw_mouse_trail_;
	bool draw_bbs_;
	fcppt::signal::scoped_connection transit_to_paused_connection_;
	fruitapp::sword_trail sword_trail_;

	void
	draw_fruit_bbs(
		sge::line_drawer::line_sequence &);

	void
	draw_mouse_trail(
		sge::line_drawer::line_sequence &);

	void
	process_fruit(
		fruit::object const &);
};
}
}
}

#endif
