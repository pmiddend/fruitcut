#ifndef FRUITCUT_SANDBOX_CUT_TEST_STATES_CUT_HPP_INCLUDED
#define FRUITCUT_SANDBOX_CUT_TEST_STATES_CUT_HPP_INCLUDED

#include "ingame.hpp"
#include "../events/render_overlay.hpp"
#include "../events/toggle_mode.hpp"
#include "../../../particle/sprite/system.hpp"
#include "../../../particle/sprite/object.hpp"
#include <sge/input/mouse/axis_event_fwd.hpp>
#include <sge/input/mouse/button_event_fwd.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/signal/scoped_connection.hpp>

namespace fruitcut
{
namespace sandbox
{
namespace cut_test
{
namespace states
{
class cut
:
	public boost::statechart::state<cut,ingame>
{
public:
	typedef 
	boost::mpl::vector2
	<
		boost::statechart::custom_reaction<events::render_overlay>,
		boost::statechart::custom_reaction<events::toggle_mode>
	> 
	reactions;

	explicit
	cut(
		my_context);

	boost::statechart::result
	react(
		events::render_overlay const &);

	boost::statechart::result
	react(
		events::toggle_mode const &);
private:
	particle::sprite::system ss_;
	particle::sprite::object cursor_;
	fcppt::signal::scoped_connection 
		mouse_axis_connection_,mouse_button_connection_;
	particle::sprite::object point1_,point2_;

	void
	mouse_axis_callback(
		sge::input::mouse::axis_event const &);

	void
	mouse_button_callback(
		sge::input::mouse::button_event const &);
};
}
}
}
}

#endif
