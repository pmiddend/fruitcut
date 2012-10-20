#include <fruitapp/config_variables.hpp>
#include <fruitapp/scene.hpp>
#include <fruitapp/shadow_map.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/depths/root.hpp>
#include <fruitapp/depths/scene.hpp>
#include <fruitapp/point_sprite/system_node.hpp>
#include <fruitapp/states/ingame/running.hpp>
#include <fruitapp/states/ingame/superstate.hpp>
#include <fruitlib/audio/music_controller.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/physics/box.hpp>
#include <fruitlib/physics/matrix4.hpp>
#include <fruitlib/physics/scalar.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <fruitlib/physics/rigid_body/parameters.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/camera/first_person/object.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/convert.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include <boost/statechart/event_base.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitapp::states::ingame::superstate::superstate(
	my_context ctx)
:
	my_base(
		ctx),
	physics_world_(
		// The box is ignored for now
		fruitlib::physics::box(),
		sge::parse::json::find_and_convert_member<fruitlib::physics::vector3>(
			context<machine>().config_file(),
			sge::parse::json::path(
				FCPPT_TEXT("physics"))
				/ FCPPT_TEXT("default-gravity"))),
	physics_world_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care))),
		context<fruitapp::machine>().ingame_clock_callback(),
		physics_world_),
	physics_debugger_(
		physics_world_,
		context<machine>().systems().renderer_ffp(),
		context<machine>().camera()),
	physics_debugger_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().overlay_node(),
				fruitlib::scenic::depth(
					depths::overlay::dont_care))),
		physics_debugger_),
	physics_debugger_connection_(
		context<machine>().systems().keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f3,
				std::tr1::bind(
					&superstate::toggle_physics_debugger,
					this)))),
	fruit_manager_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care))),
		context<machine>().fruit_prototypes(),
		context<machine>().systems().renderer_ffp(),
		physics_world_,
		context<machine>().camera(),
		context<fruitapp::machine>().ingame_clock()),
	fruit_default_render_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().scene_node(),
				fruitlib::scenic::depth(
					depths::scene::fruits))),
		context<machine>().shader_context(),
		fruit_manager_,
		context<machine>().camera(),
		context<machine>().main_light_source(),
		fruitapp::ambient_intensity(
			sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
				context<machine>().config_file(),
				sge::parse::json::path(
					FCPPT_TEXT("ambient-intensity"))))),
	fruit_shadow_render_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().shadow_map(),
				fruitlib::scenic::depth(
					0))),
		context<machine>().shader_context(),
		fruit_manager_,
		context<machine>().shadow_map().mvp()),
	fruit_spawner_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care))),
		fruit_manager_,
		context<machine>().random_generator(),
		context<machine>().config_file(),
		context<machine>().camera(),
		context<machine>().ingame_clock(),
		context<machine>().projection_manager()),
	game_logic_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care))),
		context<fruitapp::machine>().systems().renderer_ffp(),
		context<fruitapp::machine>().ingame_clock(),
		context<fruitapp::machine>().config_file(),
		context<fruitapp::machine>().sound_controller(),
		fruit_manager_,
		context<fruitapp::machine>().font_cache(),
		context<fruitapp::machine>().overlay_node(),
		context<fruitapp::machine>().viewport_manager()),
	cut_connection_(
		fruit_manager_.cut_callback(
			std::tr1::bind(
				&superstate::fruit_was_cut,
				this,
				std::tr1::placeholders::_1))),
	splatter_generator_(
		context<fruitapp::machine>().config_file(),
		context<fruitapp::machine>().config_variables().splatter_count_to_area_factor(),
		context<fruitapp::machine>().point_sprites(),
		context<fruitapp::machine>().random_generator(),
		fruitapp::point_sprite::splatter::acceleration(
			fcppt::math::vector::structure_cast<fruitapp::point_sprite::splatter::acceleration::value_type>(
				physics_world_.gravity())),
				context<fruitapp::machine>().ingame_clock()),
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
				btVector3(0,0,-1),
				0),
			fruitlib::physics::rigid_body::solidity::solid,
			fruitlib::physics::rigid_body::optional_mass(),
			fruitlib::physics::rigid_body::user_data())),
	background_body_scope_()
{
	// scene
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

fruitlib::physics::world &
fruitapp::states::ingame::superstate::physics_world()
{
	return physics_world_;
}

fruitlib::physics::world const &
fruitapp::states::ingame::superstate::physics_world() const
{
	return physics_world_;
}

fruitapp::fruit::manager &
fruitapp::states::ingame::superstate::fruit_manager()
{
	return fruit_manager_;
}

fruitapp::fruit::manager const &
fruitapp::states::ingame::superstate::fruit_manager() const
{
	return fruit_manager_;
}

fruitapp::fruit::spawner &
fruitapp::states::ingame::superstate::fruit_spawner()
{
	return fruit_spawner_;
}

fruitapp::fruit::spawner const &
fruitapp::states::ingame::superstate::fruit_spawner() const
{
	return fruit_spawner_;
}

fruitlib::physics::debugger &
fruitapp::states::ingame::superstate::physics_debugger()
{
	return physics_debugger_;
}

fruitapp::game_logic::object &
fruitapp::states::ingame::superstate::game_logic()
{
	return game_logic_;
}

fruitapp::game_logic::object const &
fruitapp::states::ingame::superstate::game_logic() const
{
	return game_logic_;
}

fruitapp::states::ingame::superstate::~superstate()
{
}

// FIXME: This could be a nice phoenix actor
void
fruitapp::states::ingame::superstate::toggle_physics_debugger()
{
	physics_debugger_.active(
		!physics_debugger_.active());
}

void
fruitapp::states::ingame::superstate::fruit_was_cut(
	fruit::cut_context const &ccontext)
{
	context<machine>().sound_controller().play(
		fruitlib::resource_tree::path(
			FCPPT_TEXT("fruit_was_cut")));

	splatter_generator_.fruit_was_cut(
		ccontext);
}
