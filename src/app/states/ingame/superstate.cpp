#include "superstate.hpp"
#include "running.hpp"
#include "../../point_sprite/system_node.hpp"
#include "../../shadow_map.hpp"
#include "../../scene.hpp"
#include "../../../fruitlib/audio/music_controller.hpp"
#include "../../../fruitlib/audio/sound_controller.hpp"
#include "../../../fruitlib/resource_tree/path.hpp"
#include "../../../fruitlib/input/state.hpp"
#include "../../../fruitlib/json/find_member.hpp"
#include "../../../fruitlib/json/parse_color.hpp"
#include "../../../fruitlib/physics/vector3.hpp"
#include "../../../fruitlib/physics/rigid_body/parameters.hpp"
#include "../../../fruitlib/physics/vector3.hpp"
#include "../../../fruitlib/physics/matrix4.hpp"
#include "../../../fruitlib/physics/scalar.hpp"
#include "../../../fruitlib/physics/box.hpp"
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/systems/instance.hpp>
#include <sge/time/activation_state.hpp>
#include <sge/time/second.hpp>
#include <sge/time/unit.hpp>
#include <sge/time/second.hpp>
#include <sge/image/color/convert.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image/colors.hpp>
#include <fcppt/math/box/box.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/text.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/lexical_cast.hpp>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include <boost/statechart/event_base.hpp>
#include <iostream>

fruitcut::app::states::ingame::superstate::superstate(
	my_context ctx)
:
	my_base(
		ctx),
	physics_world_(
		// The box is ignored for now
		fruitlib::physics::box(),
		fruitlib::json::find_member<fruitlib::physics::vector3>(
			context<machine>().config_file(),
			FCPPT_TEXT("physics/gravity"))),
	physics_world_node_(
		physics_world_,
		context<machine>().timer_callback()),
	physics_debugger_(
		physics_world_,
		context<machine>().systems().renderer(),
		context<machine>().camera()),
	physics_debugger_node_(
		physics_debugger_),
	physics_debugger_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f3, 
				std::tr1::bind(
					&superstate::toggle_physics_debugger,
					this)))),
	fruit_manager_(
		context<machine>().fruit_prototypes(),
		context<machine>().systems().renderer(),
		physics_world_,
		context<machine>().camera()),
	fruit_default_render_node_(
		context<machine>().systems().renderer(),
		fruit_manager_.vertex_declaration(),
		fruit_manager_,
		context<machine>().camera(),
		context<machine>().main_light_source(),
		fruitlib::json::find_member<sge::renderer::scalar>(
			context<machine>().config_file(),
			FCPPT_TEXT("ambient-intensity"))),
	fruit_shadow_render_node_(
		context<machine>().systems().renderer(),
		fruit_manager_.vertex_declaration(),
		fruit_manager_,
		context<machine>().shadow_map().mvp()),
	fruit_spawner_(
		fruit_manager_,
		context<machine>().rng_creator(),
		context<machine>().config_file(),
		context<machine>().camera(),
		context<machine>().timer_callback()),
	game_logic_(
		context<machine>().timer_callback(),
		context<machine>().config_file(),
		fruit_manager_,
		context<machine>().font_cache(),
		context<machine>().overlay_node(),
		context<machine>().systems().renderer(),
		context<machine>().systems().viewport_manager()),
	cut_connection_(
		fruit_manager_.cut_callback(
			std::tr1::bind(
				&superstate::fruit_was_cut,
				this,
				std::tr1::placeholders::_1))),
	splatter_generator_(
		context<machine>().config_file(),
		context<machine>().point_sprites(),
		context<machine>().rng_creator(),
		point_sprite::splatter::acceleration(
			fcppt::math::vector::structure_cast<point_sprite::splatter::acceleration::value_type>(
				physics_world_.gravity())),
		context<machine>().timer_callback()),
	background_group_(
		physics_world_),
	background_physics_(
		fruitlib::physics::rigid_body::parameters(
			fruitlib::physics::rigid_body::position(
				fruitlib::physics::vector3(
					0,
					0,
					0)),
			fruitlib::physics::rigid_body::transformation(
				fruitlib::physics::matrix4::identity()),
			fruitlib::physics::rigid_body::linear_velocity(
				fruitlib::physics::vector3::null()),
			fruitlib::physics::rigid_body::angular_velocity(
				fruitlib::physics::vector3::null()),
			fcppt::make_shared_ptr<btStaticPlaneShape>(
				btVector3(0,0,1),
				0),
			fruitlib::physics::rigid_body::solidity::solid,
			fruitlib::physics::rigid_body::optional_mass(),
			fruitlib::physics::rigid_body::user_data())),
	background_body_scope_()
{
	// scene
	context<machine>().scene_node().insert_dont_care(
		fruit_manager_);
	context<machine>().scene_node().insert_before(
		fruit_default_render_node_,
		context<machine>().point_sprites());
	context<machine>().scene_node().insert_dont_care(
		fruit_spawner_);
	context<machine>().shadow_map().insert_dont_care(
		fruit_shadow_render_node_);
	context<machine>().scene_node().insert_dont_care(
		game_logic_);
	context<machine>().scene_node().insert_dont_care(
		physics_world_node_);
	// overlay
	context<machine>().overlay_node().insert_dont_care(
		physics_debugger_node_);

	context<machine>().music_controller().play(
		fruitlib::resource_tree::path(
			FCPPT_TEXT("random")));

	physics_world_.make_groups_collide(
		background_group_,
		fruit_manager_.fruit_group());

	background_body_scope_.take(
		fcppt::make_unique_ptr<fruitlib::physics::rigid_body::scoped>(
			fcppt::ref(
				physics_world_),
			fcppt::ref(
				background_physics_),
			fcppt::assign::make_container<fruitlib::physics::group::sequence>(
				fcppt::ref(
					background_group_))));
}

fruitcut::fruitlib::physics::world &
fruitcut::app::states::ingame::superstate::physics_world()
{
	return physics_world_;
}

fruitcut::fruitlib::physics::world const &
fruitcut::app::states::ingame::superstate::physics_world() const
{
	return physics_world_;
}

fruitcut::app::fruit::manager &
fruitcut::app::states::ingame::superstate::fruit_manager()
{
	return fruit_manager_;
}

fruitcut::app::fruit::manager const &
fruitcut::app::states::ingame::superstate::fruit_manager() const
{
	return fruit_manager_;
}

fruitcut::app::fruit::spawner &
fruitcut::app::states::ingame::superstate::fruit_spawner()
{
	return fruit_spawner_;
}

fruitcut::app::fruit::spawner const &
fruitcut::app::states::ingame::superstate::fruit_spawner() const
{
	return fruit_spawner_;
}

fruitcut::fruitlib::physics::debugger &
fruitcut::app::states::ingame::superstate::physics_debugger()
{
	return physics_debugger_;
}

fruitcut::app::game_logic::game_logic &
fruitcut::app::states::ingame::superstate::game_logic()
{
	return game_logic_;
}

fruitcut::app::game_logic::game_logic const &
fruitcut::app::states::ingame::superstate::game_logic() const
{
	return game_logic_;
}

fruitcut::app::states::ingame::superstate::~superstate()
{
}

// FIXME: This could be a nice phoenix actor
void
fruitcut::app::states::ingame::superstate::toggle_physics_debugger()
{
	physics_debugger_.active(
		!physics_debugger_.active());
}

void
fruitcut::app::states::ingame::superstate::fruit_was_cut(
	fruit::cut_context const &ccontext)
{
	context<machine>().sound_controller().play(
		fruitlib::resource_tree::path(
			FCPPT_TEXT("fruit_was_cut")));

	splatter_generator_.fruit_was_cut(
		ccontext);
}
