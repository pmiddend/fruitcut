#include <fruitapp/quick_log.hpp>
#include <fruitapp/scene.hpp>
#include <fruitapp/config_variables/object.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/depths/root.hpp>
#include <fruitapp/depths/scene.hpp>
#include <fruitapp/fruit/callbacks/cut.hpp>
#include <fruitapp/point_sprite/system_node.hpp>
#include <fruitapp/shadow_map/object.hpp>
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
#include <sge/input/key/action_callback.hpp>
#include <sge/input/key/code.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/make_ref.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/reference_impl.hpp>
#include <fcppt/text.hpp>
#include <fcppt/cast/size_fun.hpp>
#include <fcppt/math/box/null.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/math/matrix/identity.hpp>
#include <fcppt/math/vector/null.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/optional/map.hpp>
#include <fcppt/config/external_begin.hpp>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include <boost/statechart/event_base.hpp>
#include <functional>
#include <memory>
#include <fcppt/config/external_end.hpp>


fruitapp::states::ingame::superstate::superstate(
	my_context ctx)
:
	my_base(
		ctx),
	physics_world_(
		// The box is ignored for now
		fcppt::math::box::null<
			fruitlib::physics::box
		>(),
		sge::parse::json::find_and_convert_member<fruitlib::physics::vector3>(
			this->context<fruitapp::machine>().config_file(),
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
		this->context<fruitapp::machine>().systems().renderer_device_ffp(),
		this->context<fruitapp::machine>().camera()),
	physics_debugger_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().overlay_node(),
				fruitlib::scenic::depth(
					depths::overlay::dont_care))),
		physics_debugger_),
	physics_debugger_connection_(
		this->context<fruitapp::machine>().systems().keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::key::code::f3,
				sge::input::key::action_callback{
					std::bind(
						&superstate::toggle_physics_debugger,
						this
					)
				}
			)
		)
	),
	fruit_manager_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care))),
		this->context<fruitapp::machine>().fruit_prototypes(),
		this->context<fruitapp::machine>().systems().renderer_device_ffp(),
		physics_world_,
		this->context<fruitapp::machine>().camera(),
		context<fruitapp::machine>().ingame_clock()),
	fruit_default_render_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().scene_node(),
				fruitlib::scenic::depth(
					depths::scene::fruits))),
		this->context<fruitapp::machine>().systems().renderer_device_core(),
		this->context<fruitapp::machine>().graphics_settings(),
		this->context<fruitapp::machine>().shader_context(),
		fruit_manager_,
		this->context<fruitapp::machine>().camera(),
		this->context<fruitapp::machine>().light_manager()),
	fruit_shadow_render_node_(
		fcppt::optional::map(
			context<fruitapp::machine>().shadow_map(),
			[
				this
			](
				fcppt::reference<
					fruitapp::shadow_map::object
				> const _shadow_map
			)
			{
				return
					fcppt::make_unique_ptr<fruitapp::fruit::rendering::shadow_node>(
						fruitlib::scenic::optional_parent(
							fruitlib::scenic::parent(
								_shadow_map.get(),
								fruitlib::scenic::depth(
									0))),
						context<fruitapp::machine>().shader_context().get_unsafe().get(), // TODO
						fruit_manager_,
						_shadow_map.get().mvp());
			}
		)
	),
	fruit_spawner_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care))),
		fruit_manager_,
		this->context<fruitapp::machine>().random_generator(),
		sge::parse::json::find_and_convert_member<sge::parse::json::object>(
			this->context<fruitapp::machine>().config_file(),
			sge::parse::json::path(
				FCPPT_TEXT("fruit-spawner"))),
		this->context<fruitapp::machine>().camera(),
		this->context<fruitapp::machine>().ingame_clock(),
		this->context<fruitapp::machine>().projection_manager()),
	game_logic_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care))),
		context<fruitapp::machine>().projection_manager(),
		context<fruitapp::machine>().ingame_clock(),
		context<fruitapp::machine>().config_file(),
		context<fruitapp::machine>().sound_controller(),
		fruit_manager_,
		context<fruitapp::machine>().font_manager(),
		context<fruitapp::machine>().overlay_node(),
		context<fruitapp::machine>().viewport_manager()),
	cut_connection_(
		fruit_manager_.cut_callback(
			fruitapp::fruit::callbacks::cut{
				std::bind(
					&superstate::fruit_was_cut,
					this,
					std::placeholders::_1
				)
			}
		)
	),
	splatter_generator_(
		sge::parse::json::find_and_convert_member<sge::parse::json::object>(
			this->context<fruitapp::machine>().config_file(),
			sge::parse::json::path(
				FCPPT_TEXT("splatter-generator"))),
		context<fruitapp::machine>().point_sprites(),
		context<fruitapp::machine>().random_generator(),
		fruitapp::point_sprite::splatter::acceleration(
			fcppt::math::vector::structure_cast<fruitapp::point_sprite::splatter::acceleration::value_type, fcppt::cast::size_fun>(
				physics_world_.gravity())),
				context<fruitapp::machine>().ingame_clock()),
	background_group_(
		physics_world_),
	background_physics_(
		fruitlib::physics::rigid_body::parameters(
			fruitlib::physics::rigid_body::position(
				fcppt::math::vector::null<
					fruitlib::physics::vector3
				>()
			),
			fruitlib::physics::rigid_body::transformation(
				fcppt::math::matrix::identity<
					fruitlib::physics::matrix4
				>()
			),
			fruitlib::physics::rigid_body::linear_velocity(
				fcppt::math::vector::null<
					fruitlib::physics::vector3
				>()
			),
			fruitlib::physics::rigid_body::angular_velocity(
				fcppt::math::vector::null<
					fruitlib::physics::vector3
				>()
			),
			fcppt::make_shared_ptr<btStaticPlaneShape>(
				btVector3(0,0,-1),
				0),
			fruitlib::physics::rigid_body::solidity::solid,
			fruitlib::physics::rigid_body::optional_mass(),
			fruitlib::physics::rigid_body::user_data())),
	background_body_scope_()
{
	// scene
	this->context<fruitapp::machine>().music_controller().play(
		fruitlib::resource_tree::path(
			FCPPT_TEXT("random")));

	physics_world_.make_groups_collide(
		background_group_,
		fruit_manager_.fruit_group());

	// TODO: initialize this directly
	background_body_scope_ =
		std::make_unique<fruitlib::physics::rigid_body::scoped>(
			physics_world_,
			background_physics_,
			fruitlib::physics::group::sequence{
				fcppt::make_ref(
					background_group_)});
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
	fruitapp::fruit::cut_context const &ccontext)
{
	this->context<fruitapp::machine>().sound_controller().play(
		fruitlib::resource_tree::path(
			FCPPT_TEXT("fruit_was_cut")));

	splatter_generator_.fruit_was_cut(
		ccontext);
}
