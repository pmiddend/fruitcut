#ifndef FRUITAPP_STATES_INGAME_SUPERSTATE_HPP_INCLUDED
#define FRUITAPP_STATES_INGAME_SUPERSTATE_HPP_INCLUDED

#include <fruitapp/machine.hpp>
#include <fruitapp/splatter_generator.hpp>
#include <fruitapp/fruit/cut_context_fwd.hpp>
#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/fruit/object_fwd.hpp>
#include <fruitapp/fruit/spawner.hpp>
#include <fruitapp/fruit/rendering/draw_node.hpp>
#include <fruitapp/fruit/rendering/shadow_node.hpp>
#include <fruitapp/game_logic/object.hpp>
#include <fruitapp/states/ingame/running_fwd.hpp>
#include <fruitlib/physics/debugger.hpp>
#include <fruitlib/physics/world.hpp>
#include <fruitlib/physics/group/object.hpp>
#include <fruitlib/physics/nodes/debugger.hpp>
#include <fruitlib/physics/nodes/world.hpp>
#include <fruitlib/physics/rigid_body/scoped.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/statechart/state.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


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

	fruitapp::fruit::manager &
	fruit_manager();

	fruitapp::fruit::manager const &
	fruit_manager() const;

	fruitapp::fruit::spawner &
	fruit_spawner();

	fruitapp::fruit::spawner const &
	fruit_spawner() const;

	fruitlib::physics::debugger &
	physics_debugger();

	fruitapp::game_logic::object &
	game_logic();

	fruitapp::game_logic::object const &
	game_logic() const;

	virtual
	~superstate();
private:
	fruitlib::physics::world physics_world_;
	fruitlib::physics::nodes::world physics_world_node_;
	fruitlib::physics::debugger physics_debugger_;
	fruitlib::physics::nodes::debugger physics_debugger_node_;
	fcppt::signal::auto_connection physics_debugger_connection_;
	fruitapp::fruit::manager fruit_manager_;
	fruitapp::fruit::rendering::draw_node fruit_default_render_node_;
	typedef
	fcppt::optional::object<
		fcppt::unique_ptr<
			fruitapp::fruit::rendering::shadow_node
		>
	>
	optional_fruit_shadow_render_node;
	optional_fruit_shadow_render_node const fruit_shadow_render_node_;
	fruitapp::fruit::spawner fruit_spawner_;
	fruitapp::game_logic::object game_logic_;
	fcppt::signal::auto_connection cut_connection_;
	fruitapp::splatter_generator splatter_generator_;
	fruitlib::physics::group::object background_group_;
	fruitlib::physics::rigid_body::object background_physics_;
	std::unique_ptr<fruitlib::physics::rigid_body::scoped> background_body_scope_;

	void
	toggle_physics_debugger();

	void
	fruit_was_cut(
		fruitapp::fruit::cut_context const &);
};
}
}
}

#endif
