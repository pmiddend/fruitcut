#ifndef FRUITCUT_APP_STATES_INGAME_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INGAME_HPP_INCLUDED

#include "running_fwd.hpp"
#include "../machine.hpp"
#include "../events/viewport_change.hpp"
#include "../fruit/manager.hpp"
#include "../../physics/world.hpp"
#include "../../physics/null_collision_filter.hpp"
#include "../../physics/debugger.hpp"
#include "../../input/state.hpp"
#include <sge/camera/object.hpp>
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
class ingame
:
	// The second argument has to be a complete type, the third one
	// doesn't have to be
	public boost::statechart::state<ingame,machine,running>
{
public:
	typedef
	boost::mpl::vector1
	<
		boost::statechart::custom_reaction<events::viewport_change>
	>
	reactions;

	explicit
	ingame(
		my_context);

	boost::statechart::result
	react(
		events::viewport_change const &);

	physics::world &
	physics_world();

	physics::world const &
	physics_world() const;

	sge::camera::object &
	camera();

	sge::camera::object const &
	camera() const;

	fruit::manager &
	fruit_manager();

	fruit::manager const &
	fruit_manager() const;

	physics::debugger &
	physics_debugger();

	~ingame();
private:
	fcppt::signal::scoped_connection toggle_pause_connection_,toggle_camera_connection_;
	input::state camera_state_;
	sge::camera::object camera_;
	physics::world physics_world_;
	physics::debugger physics_debugger_;
	fcppt::signal::scoped_connection physics_debugger_connection_;
	physics::null_collision_filter collision_filter_;
	fruit::manager fruit_manager_;

	void
	toggle_camera();

	void
	toggle_physics_debugger();
};
}
}
}

#endif
