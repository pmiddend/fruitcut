#ifndef FRUITCUT_APP_STATES_INGAME_SUPERSTATE_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INGAME_SUPERSTATE_HPP_INCLUDED

#include "running_fwd.hpp"
#include "../../splatter_generator.hpp"
#include "../../game_logic.hpp"
#include "../../machine.hpp"
#include "../../fruit/manager.hpp"
#include "../../fruit/object_fwd.hpp"
#include "../../fruit/spawner.hpp"
#include "../../fruit/cut_context_fwd.hpp"
#include "../../../fruitlib/physics/world.hpp"
#include "../../../fruitlib/physics/nodes/world.hpp"
#include "../../../fruitlib/physics/nodes/debugger.hpp"
#include "../../../fruitlib/physics/group/object.hpp"
#include "../../../fruitlib/physics/rigid_body/scoped_body.hpp"
#include "../../../fruitlib/physics/debugger.hpp"
#include "../../../fruitlib/input/state.hpp"
#include <sge/time/timer.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/scoped_ptr.hpp>
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

	fruitlib::physics::world &
	physics_world();

	fruitlib::physics::world const &
	physics_world() const;

	fruit::manager &
	fruit_manager();

	fruit::manager const &
	fruit_manager() const;

	fruit::spawner &
	fruit_spawner();

	fruit::spawner const &
	fruit_spawner() const;

	fruitlib::physics::debugger &
	physics_debugger();

	fruitcut::app::game_logic &
	game_logic();

	fruitcut::app::game_logic const &
	game_logic() const;

	~superstate();
private:
	fcppt::signal::scoped_connection 
		toggle_pause_connection_;
	fruitlib::physics::world physics_world_;
	fruitlib::physics::nodes::world physics_world_node_;
	fruitlib::physics::debugger physics_debugger_;
	fruitlib::physics::nodes::debugger physics_debugger_node_;
	fcppt::signal::scoped_connection physics_debugger_connection_;
	fruit::manager fruit_manager_;
	fruit::spawner fruit_spawner_;
	fruitcut::app::game_logic game_logic_;
	fcppt::signal::scoped_connection cut_connection_;
	app::splatter_generator splatter_generator_;
	fruitlib::physics::group::object background_group_;
	fruitlib::physics::rigid_body::object background_physics_;
	fcppt::scoped_ptr<fruitlib::physics::rigid_body::scoped_body> background_body_scope_;

	void
	toggle_physics_debugger();

	void
	fruit_was_cut(
		fruit::cut_context const &);
};
}
}
}
}

#endif
