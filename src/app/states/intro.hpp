#ifndef FRUITCUT_APP_STATES_INTRO_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INTRO_HPP_INCLUDED

#include "../machine.hpp"
#include "../logo.hpp"
#include "../events/make_transition.hpp"
#include "ingame/running_fwd.hpp"
#include <sge/time/timer.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
{
namespace states
{
class intro
:
	// The second argument has to be a complete type
	public boost::statechart::state<intro,machine>,
	public fruitlib::scenic::nodes::intrusive
{
public:
	typedef
	boost::mpl::vector1
	<
		events::make_transition<ingame::running>::type
	>
	reactions;

	explicit
	intro(
		my_context);

	~intro();
private:
	sge::renderer::state::scoped scoped_render_state_;
	sge::time::timer saturation_timer_;
	sge::time::timer intro_timer_;
	fcppt::signal::scoped_connection viewport_change_connection_;
	logo logo_;

	void
	viewport_change();

	void
	update();

	void
	render();
};
}
}
}

#endif
