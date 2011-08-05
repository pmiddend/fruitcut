#ifndef FRUITAPP_STATES_INGAME_SUPERSTATE_HPP_INCLUDED
#define FRUITAPP_STATES_INGAME_SUPERSTATE_HPP_INCLUDED

#include <fruitapp/states/ingame/running_fwd.hpp>
#include <fruitapp/splatter_generator.hpp>
#include <fruitapp/game_logic/object.hpp>
#include <fruitapp/machine.hpp>
#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/fruit/object_fwd.hpp>
#include <fruitapp/fruit/spawner.hpp>
#include <fruitapp/fruit/default_render_node.hpp>
#include <fruitapp/fruit/shadow_render_node.hpp>
#include <fruitapp/fruit/cut_context_fwd.hpp>
#include <fruitlib/physics/world.hpp>
#include <fruitlib/physics/nodes/world.hpp>
#include <fruitlib/physics/nodes/debugger.hpp>
#include <fruitlib/physics/group/object.hpp>
#include <fruitlib/physics/rigid_body/scoped.hpp>
#include <fruitlib/physics/debugger.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/scoped_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/statechart/state.hpp>

namespace fruitapp
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
FCPPT_NONCOPYABLE(
	superstate);
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

	fruitapp::game_logic::object &
	game_logic();

	fruitapp::game_logic::object const &
	game_logic() const;

	~superstate();
private:
	fruitlib::physics::world physics_world_;
	fruitlib::physics::nodes::world physics_world_node_;
	fruitlib::physics::debugger physics_debugger_;
	fruitlib::physics::nodes::debugger physics_debugger_node_;
	fcppt::signal::scoped_connection physics_debugger_connection_;
	fruit::manager fruit_manager_;
	fruit::default_render_node fruit_default_render_node_;
	fruit::shadow_render_node fruit_shadow_render_node_;
	fruit::spawner fruit_spawner_;
	fruitapp::game_logic::object game_logic_;
	fcppt::signal::scoped_connection cut_connection_;
	fruitapp::splatter_generator splatter_generator_;
	fruitlib::physics::group::object background_group_;
	fruitlib::physics::rigid_body::object background_physics_;
	fcppt::scoped_ptr<fruitlib::physics::rigid_body::scoped> background_body_scope_;

	void
	toggle_physics_debugger();

	void
	fruit_was_cut(
		fruit::cut_context const &);
};
}
}
}

#endif
