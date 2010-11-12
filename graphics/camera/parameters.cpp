#include "parameters.hpp"

fruitcut::graphics::camera::parameters::parameters(
	projection::object const &_projection,
	scalar const _movement_speed,
	scalar const _rotation_speed,
	vec3 const &_position,
	input::state &_state)
:
	projection_(
		_projection),
	movement_speed_(
		_movement_speed),
	rotation_speed_(
		_rotation_speed),
	position_(
		_position),
	state_(
		_state)
{
	
}

fruitcut::graphics::camera::projection::object const &
fruitcut::graphics::camera::parameters::projection() const
{
	return projection_;
}

fruitcut::scalar
fruitcut::graphics::camera::parameters::movement_speed() const
{
	return movement_speed_;
}

fruitcut::scalar
fruitcut::graphics::camera::parameters::rotation_speed() const
{
	return rotation_speed_;
}

fruitcut::vec3 const &
fruitcut::graphics::camera::parameters::position() const
{
	return position_;
}

fruitcut::input::state &
fruitcut::graphics::camera::parameters::state() const
{
	return state_;
}
