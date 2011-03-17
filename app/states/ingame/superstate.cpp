#include "superstate.hpp"
#include "running.hpp"
#include "../../events/toggle_pause.hpp"
#include "../../../input/state.hpp"
#include "../../../json/find_member.hpp"
#include "../../../json/parse_color.hpp"
#include "../../../physics/vector3.hpp"
#include "../../../physics/box.hpp"
#include "../../../font/color_animation.hpp"
#include "../../../font/color_format.hpp"
#include "../../../font/scale_animation.hpp"
#include <sge/camera/identity_gizmo.hpp>
#include <sge/camera/parameters.hpp>
#include <sge/camera/projection/perspective.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/viewport/manager.hpp>
#include <sge/systems/instance.hpp>
#include <sge/time/activation_state.hpp>
#include <sge/time/second.hpp>
#include <sge/time/unit.hpp>
#include <sge/time/second.hpp>
#include <sge/image/color/convert.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image/colors.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/text.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/home/phoenix/core.hpp>
#include <boost/spirit/home/phoenix/operator.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/statechart/event_base.hpp>
#include <iostream>

fruitcut::app::states::ingame::superstate::superstate(
	my_context ctx)
:
	my_base(
		ctx),
	toggle_pause_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::p, 
				std::tr1::bind(
					// Note that using post_event does something unexpected. If
					// you use that, you get a tick event first and _then_ the
					// toggle_pause event, which is not the desired behaviour
					// (post_event posts to the queue, process_event immediately
					// processes it)
					&machine::process_event,
					&context<machine>(),
					events::toggle_pause())))),
	toggle_camera_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f2, 
				std::tr1::bind(
					&superstate::toggle_camera,
					this)))),
	viewport_change_connection_(
		context<machine>().systems().viewport_manager().manage_callback(
			std::tr1::bind(
				&superstate::viewport_change,
				this))),
	camera_state_(
		context<machine>().input_manager()),
	camera_(
		sge::camera::parameters(
			// Leave projection object empty for now, we have to wait for a viewport change
			sge::camera::projection::object(),
			json::find_member<sge::renderer::scalar>(
				context<machine>().config_file(),
				FCPPT_TEXT("ingame/camera/movement-speed")),
			// mousespeed
			json::find_member<sge::renderer::scalar>(
				context<machine>().config_file(),
				FCPPT_TEXT("ingame/camera/mouse-speed")),
			// position
			sge::camera::identity_gizmo()
				.position(
					json::find_member<sge::renderer::vector3>(
						context<machine>().config_file(),
						FCPPT_TEXT("ingame/camera/initial-position"))),
			// Maus und Keyboard
			context<machine>().game_input_state(),
			context<machine>().game_input_state(),
			sge::camera::activation_state::inactive)),
	physics_world_(
		// The box is ignored for now
		physics::box(),
		json::find_member<physics::vector3>(
			context<machine>().config_file(),
			FCPPT_TEXT("physics/gravity"))),
	physics_debugger_(
		physics_world_,
		context<machine>().systems().renderer(),
		camera_),
	physics_debugger_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f3, 
				std::tr1::bind(
					&superstate::toggle_physics_debugger,
					this)))),
	collision_filter_(
		physics_world_),
	fruit_manager_(
		json::find_member<sge::parse::json::array>(
			context<machine>().config_file(),
			FCPPT_TEXT("fruits")),
		*context<machine>().systems().md3_loader(),
		context<machine>().systems().image_loader(),
		context<machine>().systems().renderer(),
		physics_world_),
	fruit_spawner_(
		fruit_manager_,
		context<machine>().config_file(),
		camera_,
		context<machine>().timer_callback()),
	font_system_(
		context<machine>().font_cache()),
	game_logic_(
		context<machine>().timer_callback(),
		context<machine>().config_file(),
		fruit_manager_,
		font_system_,
		*context<machine>().systems().renderer(),
		context<machine>().systems().viewport_manager()),
	cut_connection_(
		fruit_manager_.cut_callback(
			std::tr1::bind(
				&superstate::fruit_was_cut,
				this,
				std::tr1::placeholders::_1,
				std::tr1::placeholders::_2,
				std::tr1::placeholders::_3,
				std::tr1::placeholders::_4)))
{
	viewport_change();
}

fruitcut::physics::world &
fruitcut::app::states::ingame::superstate::physics_world()
{
	return physics_world_;
}

fruitcut::physics::world const &
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

fruitcut::font::system &
fruitcut::app::states::ingame::superstate::font_system()
{
	return font_system_;
}

fruitcut::font::system const &
fruitcut::app::states::ingame::superstate::font_system() const
{
	return font_system_;
}

fruitcut::app::fruit::spawner const &
fruitcut::app::states::ingame::superstate::fruit_spawner() const
{
	return fruit_spawner_;
}

fruitcut::physics::debugger &
fruitcut::app::states::ingame::superstate::physics_debugger()
{
	return physics_debugger_;
}

fruitcut::app::game_logic &
fruitcut::app::states::ingame::superstate::game_logic()
{
	return game_logic_;
}

fruitcut::app::game_logic const &
fruitcut::app::states::ingame::superstate::game_logic() const
{
	return game_logic_;
}

sge::camera::object &
fruitcut::app::states::ingame::superstate::camera()
{
	return camera_;
}

sge::camera::object const &
fruitcut::app::states::ingame::superstate::camera() const
{
	return camera_;
}

fruitcut::app::states::ingame::superstate::~superstate()
{
}

// FIXME: This could be a nice phoenix actor
void
fruitcut::app::states::ingame::superstate::toggle_camera()
{
	camera_.activation(
		camera_.activation() == sge::camera::activation_state::active
		?
			sge::camera::activation_state::inactive
		:
			sge::camera::activation_state::active);
}

// FIXME: This could be a nice phoenix actor
void
fruitcut::app::states::ingame::superstate::toggle_physics_debugger()
{
	physics_debugger_.active(
		!physics_debugger_.active());
}

void
fruitcut::app::states::ingame::superstate::viewport_change()
{
	camera_.projection_object(
		sge::camera::projection::perspective(
			sge::renderer::aspect(
				fcppt::math::dim::structure_cast<sge::renderer::screen_size>(
					context<machine>().systems().renderer()->onscreen_target()->viewport().get().size())),
			fcppt::math::deg_to_rad(
				json::find_member<sge::renderer::scalar>(
					context<machine>().config_file(),
					FCPPT_TEXT("ingame/camera/fov"))),
			json::find_member<sge::renderer::scalar>(
				context<machine>().config_file(),
				FCPPT_TEXT("ingame/camera/near")),
			json::find_member<sge::renderer::scalar>(
				context<machine>().config_file(),
				FCPPT_TEXT("ingame/camera/far"))));
}

void
fruitcut::app::states::ingame::superstate::fruit_was_cut(
	fruit::object const &,
	fruit::object const &,
	fruit::object const &,
	sge::renderer::scalar const _area)
{
	/*
	score_ = 
		static_cast<fruitcut::app::score>(
			static_cast<sge::renderer::scalar>(
				score_) + 1000 * _area);
	score_font_.text(
		boost::lexical_cast<sge::font::text::string>(
			score_));
	*/
	context<machine>().sound_controller().play(
		FCPPT_TEXT("fruit-was-cut"));
}
