#include "object.hpp"
#include "parameters.hpp"
#include "../../math/gizmo/to_rotation_matrix.hpp"
#include "../../input/state.hpp"
#include "projection/to_matrix.hpp"
#include <sge/input/keyboard/key_code.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <sge/input/mouse/axis_event.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/rotation_axis.hpp>
#include <functional>
#include <algorithm>

fruitcut::graphics::camera::object::object(
	parameters const &params)
:
	state_connection_(
		params.state().create_connection(
			input::optional_key_callback(
				std::bind(
					&object::key_callback,
					this,
					std::placeholders::_1)),
			input::optional_mouse_axis_callback(
				std::bind(
					&object::mouse_axis_callback,
					this,
					std::placeholders::_1)),
			input::optional_mouse_button_callback())),
	projection_(
		params.projection()),
	projection_matrix_(
		projection::to_matrix(
			projection_)),
	movement_speed_(
		params.movement_speed()),
	rotation_speed_(
		params.rotation_speed()),
	gizmo_(
		fruitcut::gizmo()
			.position(
				params.position())
			.forward(
				vec3(0,0,1))
			.up(
				vec3(0,1,0))
			.right(
				vec3(1,0,0))),
	dirs_(
		vec3::null())
{
}

void
fruitcut::graphics::camera::object::update(
	scalar const time_delta)
{
	gizmo_.position( 
		gizmo_.position() + 
		movement_speed_ * 
		time_delta * 
		std::inner_product(
			gizmo_.array().begin(),
			gizmo_.array().end(),
			dirs_.data(),
			vec3::null()));
}

fruitcut::mat4 const
fruitcut::graphics::camera::object::world() const
{
	return 
		rotation() * 
		translation();
}

fruitcut::mat4 const
fruitcut::graphics::camera::object::rotation() const
{
	return 
		math::gizmo::to_rotation_matrix(
			gizmo_);
}

fruitcut::mat4 const
fruitcut::graphics::camera::object::translation() const
{
	return 
		fcppt::math::matrix::translation(
			-gizmo_.position());
}

fruitcut::mat4 const
fruitcut::graphics::camera::object::projection() const
{
	return projection_matrix_;
}

fruitcut::mat4 const 
fruitcut::graphics::camera::object::mvp() const
{
	return
		projection() * world();
}

fruitcut::gizmo const &
fruitcut::graphics::camera::object::gizmo() const
{
	return gizmo_;
}

fruitcut::gizmo &
fruitcut::graphics::camera::object::gizmo()
{
	return gizmo_;
}

void
fruitcut::graphics::camera::object::key_callback(
	sge::input::keyboard::key_event const &k)
{
	switch (k.key().code())
	{
		case sge::input::keyboard::key_code::space:
			dirs_[1] = !k.pressed() ? 0 : 1;
			break;
		case sge::input::keyboard::key_code::lctrl:
			dirs_[1] = !k.pressed() ? 0 : -1;
			break;
		case sge::input::keyboard::key_code::up:
			dirs_[2] = !k.pressed() ? 0 : -1;
			break;
		case sge::input::keyboard::key_code::down:
			dirs_[2] = !k.pressed() ? 0 : 1;
			break;
		case sge::input::keyboard::key_code::left:
			dirs_[0] = !k.pressed() ? 0 : -1;
			break;
		case sge::input::keyboard::key_code::right:
			dirs_[0] = !k.pressed() ? 0 : 1;
			break;
		default:
			break;
	}
}

void
fruitcut::graphics::camera::object::mouse_axis_callback(
	sge::input::mouse::axis_event const &k)
{
	scalar const angle = 
		static_cast<scalar>(k.axis_position())/rotation_speed_;

	switch (k.axis())
	{
		case sge::input::mouse::axis::x:
		{
		using fcppt::math::matrix::rotation_axis;
		using fcppt::math::vector::narrow_cast;
		using fcppt::math::vector::construct;
		using fcppt::math::vector::normalize;

		vec3 
			forward = 
				narrow_cast<vec3>(
					rotation_axis(
						-angle,
						vec3(0,1,0)) *
					construct(
						gizmo_.forward(),
						// Cast neccesary here
						static_cast<scalar>(0))),
			right = 
				cross(
					vec3(0,1,0),
					forward),
			up = 
				cross(
					forward,
					right); 

		gizmo_ 
			.position(gizmo_.position())
			.forward(normalize(forward))
			.up(normalize(up))
			.right(normalize(right));
		}
		break;
		case sge::input::mouse::axis::y:
		{
			using fcppt::math::matrix::rotation_axis;
			using fcppt::math::vector::narrow_cast;
			using fcppt::math::vector::construct;

			vec3 
				forward = 
					narrow_cast<vec3>(
						rotation_axis(
							-angle,
							gizmo_.right()) *
						construct(
							gizmo_.forward(),
							static_cast<scalar>(0))),
				up = 
					cross(forward,gizmo_.right()),
				right = 
					cross(up,forward); 

			gizmo_
				.position(gizmo_.position())
				.forward(normalize(forward))
				.up(normalize(up))
				.right(normalize(right));
		}
		break;
		case sge::input::mouse::axis::wheel:
		break;
	}
}
