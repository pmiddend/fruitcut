#ifndef FRUITCUT_APP_STATES_INGAME_RUNNING_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INGAME_RUNNING_HPP_INCLUDED

#include "superstate.hpp"
#include "../../fruit/object_fwd.hpp"
#include "../../events/render.hpp"
#include "../../events/tick.hpp"
#include "../../events/toggle_pause.hpp"
#include "../../events/render_overlay.hpp"
#include "../../../fruitlib/scenic/nodes/line_drawer.hpp"
#include "../../../fruitlib/cursor_trail.hpp"
#include "../../../fruitlib/scenic/nodes/cursor_trail.hpp"
#include "../../../fruitlib/scenic/nodes/intrusive_with_callbacks.hpp"
#include <sge/line_drawer/object.hpp>
#include <sge/line_drawer/line_sequence.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
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
	public boost::statechart::state<running,superstate>
{
public:
	typedef
	boost::mpl::vector2
	<
		boost::statechart::custom_reaction<events::tick>,
		boost::statechart::custom_reaction<events::toggle_pause>
	>
	reactions;
	explicit
	running(
		my_context);

	boost::statechart::result
	react(
		events::tick const &);

	boost::statechart::result
	react(
		events::toggle_pause const &);

	~running();
private:
	sge::renderer::state::scoped scoped_render_state_;
	sge::line_drawer::object line_drawer_;
	fruitlib::scenic::nodes::line_drawer line_drawer_node_;
	fruitlib::cursor_trail cursor_trail_;
	fruitlib::scenic::nodes::cursor_trail cursor_trail_node_;
	fruitlib::scenic::nodes::intrusive_with_callbacks update_node_;
	fcppt::signal::scoped_connection 
		viewport_change_connection_,
		fruit_spawned_connection_;
	bool 
		draw_mouse_trail_,
		draw_bbs_;

	void
	update();

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
