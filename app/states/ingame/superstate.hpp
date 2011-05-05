#ifndef FRUITCUT_APP_STATES_INGAME_SUPERSTATE_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INGAME_SUPERSTATE_HPP_INCLUDED

#include "running_fwd.hpp"
#include "../../../scenic/nodes/camera.hpp"
#include "../../game_logic.hpp"
#include "../../machine.hpp"
#include "../../fruit/manager.hpp"
#include "../../fruit/object_fwd.hpp"
#include "../../fruit/spawner.hpp"
#include "../../fruit/cut_context_fwd.hpp"
#include "../../../physics/world.hpp"
#include "../../../physics/nodes/world.hpp"
#include "../../../physics/nodes/debugger.hpp"
#include "../../../physics/null_collision_filter.hpp"
#include "../../../physics/debugger.hpp"
#include "../../../input/state.hpp"
#include <sge/camera/object.hpp>
#include <sge/time/timer.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state.hpp>

namespace fruitcut
{
namespace app
{
namespace states
{
namespace ingame
{
class superstate
:
	// The second argument has to be a complete type, the third one
	// doesn't have to be
	public boost::statechart::state<superstate,machine,running>
{
public:
	explicit
	superstate(
		my_context);

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

	fruit::spawner &
	fruit_spawner();

	fruit::spawner const &
	fruit_spawner() const;

	physics::debugger &
	physics_debugger();

	fruitcut::app::game_logic &
	game_logic();

	fruitcut::app::game_logic const &
	game_logic() const;

	~superstate();
private:
	fcppt::signal::scoped_connection 
		toggle_pause_connection_,
		toggle_camera_connection_,
		viewport_change_connection_;
	input::state camera_state_;
	sge::camera::object camera_;
	scenic::nodes::camera camera_node_;
	physics::world physics_world_;
	physics::nodes::world physics_world_node_;
	physics::debugger physics_debugger_;
	physics::nodes::debugger physics_debugger_node_;
	fcppt::signal::scoped_connection physics_debugger_connection_;
	physics::null_collision_filter collision_filter_;
	fruit::manager fruit_manager_;
	fruit::spawner fruit_spawner_;
	fruitcut::app::game_logic game_logic_;
	fcppt::signal::scoped_connection cut_connection_;

	void
	toggle_camera();

	void
	toggle_physics_debugger();

	void
	viewport_change();

	void
	fruit_was_cut(
		fruit::cut_context const &);
};
}
}
}
}

#endif
