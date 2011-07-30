#ifndef FRUITCUT_APP_STATES_INGAME_RUNNING_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INGAME_RUNNING_HPP_INCLUDED

#include "superstate.hpp"
#include "paused_fwd.hpp"
#include "../../sword_trail.hpp"
#include "../../cursor_trail.hpp"
#include "../gameover/superstate_fwd.hpp"
#include "../../fruit/object_fwd.hpp"
#include "../../events/declare_transition_type.hpp"
#include "../../events/declare_transition_reaction.hpp"
#include "../../../fruitlib/scenic/adaptors/line_drawer.hpp"
#include "../../../fruitlib/scenic/node.hpp"
#include "../../../fruitlib/scenic/events/update_fwd.hpp"
#include <sge/line_drawer/object.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/line_drawer/line_sequence.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/statechart/state.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
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
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			ingame::paused),
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			gameover::superstate)
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

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		ingame::paused);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		gameover::superstate);

	~running();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	sge::line_drawer::object line_drawer_;
	fruitlib::scenic::adaptors::line_drawer line_drawer_node_;
	app::cursor_trail cursor_trail_;
	fcppt::signal::scoped_connection 
		viewport_change_connection_,
		fruit_spawned_connection_;
	bool 
		draw_mouse_trail_,
		draw_bbs_;
	fcppt::signal::scoped_connection transit_to_paused_connection_;
	app::sword_trail sword_trail_;

	void
	draw_fruit_bbs(
		sge::line_drawer::line_sequence &);

	void
	draw_mouse_trail(
		sge::line_drawer::line_sequence &);

	void
	process_fruit(
		fruit::object const &);

	void
	viewport_change();
};
}
}
}
}

#endif
