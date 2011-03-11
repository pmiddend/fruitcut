#include "ingame.hpp"
#include "running.hpp"
#include "../events/toggle_pause.hpp"
#include "../../input/state.hpp"
#include "../../json/find_member.hpp"
#include "../../physics/vector3.hpp"
#include "../../physics/box.hpp"
#include "../../font/color_animation.hpp"
#include "../../font/color_format.hpp"
#include "../../font/scale_animation.hpp"
#include "../../font/particle/base_parameters.hpp"
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
#include <sge/systems/instance.hpp>
#include <sge/time/activation_state.hpp>
#include <sge/time/second.hpp>
#include <sge/time/unit.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/unit.hpp>
#include <sge/font/pos.hpp>
#include <sge/font/text/align_h.hpp>
#include <sge/font/text/align_v.hpp>
#include <sge/font/text/flags_none.hpp>
#include <sge/time/second.hpp>
#include <sge/image/color/any/convert.hpp>
#include <sge/image/colors.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/statechart/event_base.hpp>
#include <iostream>

fruitcut::app::states::ingame::ingame(
	my_context ctx)
:
	my_base(
		ctx),
	toggle_pause_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::p, 
				boost::bind(
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
				boost::bind(
					&ingame::toggle_camera,
					this)))),
	viewport_change_connection_(
		context<machine>().systems().manage_viewport_callback(
			boost::bind(
				&ingame::viewport_change,
				this,
				_1))),
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
				boost::bind(
					&ingame::toggle_physics_debugger,
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
	turn_timer_(
		sge::time::second(
			json::find_member<sge::time::unit>(
				context<machine>().config_file(),
				FCPPT_TEXT("round-seconds"))),
		sge::time::activation_state::active,
		context<machine>().timer_callback()),
	font_system_(
		context<machine>().font_cache()),
	score_font_(
		fruitcut::font::particle::base_parameters(
			font_system_,
			FCPPT_TEXT("score"),
			SGE_FONT_TEXT_LIT("0"),
			sge::font::rect::null(),
			sge::font::text::align_h::left,
			sge::font::text::align_v::top,
			sge::font::text::flags::none),
		fcppt::assign::make_container<font::color_animation::value_sequence>
			(font::color_animation::value_type(
				sge::time::second(1),
				sge::image::color::any::convert<font::color_format>(
					sge::image::colors::white()))),
		fcppt::assign::make_container<font::scale_animation::value_sequence>
			(font::scale_animation::value_type(
				sge::time::second(
					1),
				static_cast<sge::renderer::scalar>(
					1))),
		context<machine>().timer_callback()),
	score_(
		0),
	cut_connection_(
		fruit_manager_.cut_callback(
			boost::bind(
				&ingame::fruit_was_cut,
				this,
				_1)))
{
	viewport_change(
		sge::renderer::device_ptr());
}

fruitcut::physics::world &
fruitcut::app::states::ingame::physics_world()
{
	return physics_world_;
}

fruitcut::physics::world const &
fruitcut::app::states::ingame::physics_world() const
{
	return physics_world_;
}

fruitcut::app::fruit::manager &
fruitcut::app::states::ingame::fruit_manager()
{
	return fruit_manager_;
}

fruitcut::app::fruit::manager const &
fruitcut::app::states::ingame::fruit_manager() const
{
	return fruit_manager_;
}

fruitcut::app::fruit::spawner &
fruitcut::app::states::ingame::fruit_spawner()
{
	return fruit_spawner_;
}

sge::time::timer const &
fruitcut::app::states::ingame::turn_timer() const
{
	return turn_timer_;
}

fruitcut::font::system &
fruitcut::app::states::ingame::font_system()
{
	return font_system_;
}

fruitcut::font::system const &
fruitcut::app::states::ingame::font_system() const
{
	return font_system_;
}

fruitcut::app::fruit::spawner const &
fruitcut::app::states::ingame::fruit_spawner() const
{
	return fruit_spawner_;
}

fruitcut::physics::debugger &
fruitcut::app::states::ingame::physics_debugger()
{
	return physics_debugger_;
}

fruitcut::app::score
fruitcut::app::states::ingame::score() const
{
	return score_;
}

sge::camera::object &
fruitcut::app::states::ingame::camera()
{
	return camera_;
}

sge::camera::object const &
fruitcut::app::states::ingame::camera() const
{
	return camera_;
}

fruitcut::app::states::ingame::~ingame()
{
}

// FIXME: This could be a nice phoenix actor
void
fruitcut::app::states::ingame::toggle_camera()
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
fruitcut::app::states::ingame::toggle_physics_debugger()
{
	physics_debugger_.active(
		!physics_debugger_.active());
}

void
fruitcut::app::states::ingame::viewport_change(
	sge::renderer::device_ptr)
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

	sge::font::dim const &viewport_dim = 
		fcppt::math::dim::structure_cast<sge::font::dim>(
			context<machine>().systems().renderer()->onscreen_target()->viewport().get().size());

	score_font_.bounding_box(
		sge::font::rect(
			sge::font::pos::null(),
			viewport_dim));
}

void
fruitcut::app::states::ingame::fruit_was_cut(
	sge::renderer::scalar const _area)
{
	score_ = 
		static_cast<fruitcut::app::score>(
			static_cast<sge::renderer::scalar>(
				score_) + 1000 * _area);
	score_font_.text(
		boost::lexical_cast<sge::font::text::string>(
			score_));
}
