#ifndef FRUITAPP_STATES_INGAME_RUNNING_HPP_INCLUDED
#define FRUITAPP_STATES_INGAME_RUNNING_HPP_INCLUDED

#include <fruitapp/cursor/instance_fwd.hpp>
#include <fruitapp/cursor/instance_sequence_unique_ptr.hpp>
#include <fruitapp/events/declare_transition_reaction.hpp>
#include <fruitapp/events/declare_transition_type.hpp>
#include <fruitapp/fruit/object_fwd.hpp>
#include <fruitapp/states/gameover/superstate_fwd.hpp>
#include <fruitapp/states/ingame/paused_fwd.hpp>
#include <fruitapp/states/ingame/superstate.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/adaptors/line_drawer.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/line_drawer/line_sequence.hpp>
#include <sge/line_drawer/object.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/statechart/state.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace states
{
namespace ingame
{
class running
:
	// The second argument has to be a complete type
	public boost::statechart::state<running,fruitapp::states::ingame::superstate>,
	public fruitlib::scenic::node<running>
{
FCPPT_NONCOPYABLE(
	running);
public:
	typedef
	boost::mpl::vector2
	<
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			fruitapp::states::ingame::paused),
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			fruitapp::states::gameover::superstate)
	>
	reactions;

	typedef
	boost::mpl::vector1
	<
		fruitlib::scenic::events::update
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
private:
	sge::line_drawer::object line_drawer_;
	fruitlib::scenic::adaptors::line_drawer line_drawer_node_;
	fruitapp::cursor::instance_sequence_unique_ptr const cursor_sequence_;
	fcppt::signal::auto_connection fruit_spawned_connection_;
	bool draw_mouse_trail_;
/*
	bool draw_bbs_;
*/
	fcppt::signal::auto_connection transit_to_paused_connection_;

/*
	void
	draw_fruit_bbs(
		sge::line_drawer::line_sequence &);
*/

	void
	process_fruit(
		fruitapp::fruit::object const &);

	void
	process_fruit(
		fruitapp::fruit::object const &,
		fruitapp::cursor::instance const &);
};
}
}
}

#endif
