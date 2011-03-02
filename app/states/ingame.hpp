#ifndef FRUITCUT_APP_STATES_INGAME_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INGAME_HPP_INCLUDED

#include "running_fwd.hpp"
#include "../score.hpp"
#include "../machine.hpp"
#include "../fruit/manager.hpp"
#include "../fruit/spawner.hpp"
#include "../../physics/world.hpp"
#include "../../physics/null_collision_filter.hpp"
#include "../../physics/debugger.hpp"
#include "../../input/state.hpp"
#include "../../font/system.hpp"
#include "../../font/particle/animated.hpp"
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
class ingame
:
	// The second argument has to be a complete type, the third one
	// doesn't have to be
	public boost::statechart::state<ingame,machine,running>
{
public:
	explicit
	ingame(
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

	// For now, no nonconst-getter
	sge::time::timer const &
	turn_timer() const;

	font::system &
	font_system();

	font::system const &
	font_system() const;

	physics::debugger &
	physics_debugger();

	~ingame();
private:
	fcppt::signal::scoped_connection 
		toggle_pause_connection_,
		toggle_camera_connection_,
		viewport_change_connection_;
	input::state camera_state_;
	sge::camera::object camera_;
	physics::world physics_world_;
	physics::debugger physics_debugger_;
	fcppt::signal::scoped_connection physics_debugger_connection_;
	physics::null_collision_filter collision_filter_;
	fruit::manager fruit_manager_;
	fruit::spawner fruit_spawner_;
	sge::time::timer turn_timer_;
	font::system font_system_;
	font::particle::animated score_font_;
	score score_;
	fcppt::signal::scoped_connection cut_connection_;

	void
	toggle_camera();

	void
	toggle_physics_debugger();

	void
	viewport_change(
		sge::renderer::device_ptr);

	void
	fruit_was_cut(
		sge::renderer::scalar);
};
}
}
}

#endif
