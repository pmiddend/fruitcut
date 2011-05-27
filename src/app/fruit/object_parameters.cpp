#include "object_parameters.hpp"

fruitcut::app::fruit::object_parameters::object_parameters(
	sge::renderer::texture::planar_ptr const _texture,
	physics::world &_world,
	sge::renderer::device &_renderer,
	sge::renderer::vertex_declaration &_vertex_declaration,
	sge::shader::object &_shader,
	fruitcut::app::fruit::mesh const &_mesh,
	physics::scalar const _mass,
	physics::vector3 const &_position,
	physics::matrix4 const &_transformation,
	physics::vector3 const &_linear_velocity,
	physics::vector3 const &_angular_velocity,
	sge::time::duration const &_lock_duration,
	sge::time::callback const &_timer_callback)
:
	texture_(
		_texture),
	world_(
		_world),
	renderer_(
		_renderer),
	vertex_declaration_(
		_vertex_declaration),
	shader_(
		_shader),
	mesh_(
		_mesh),
	mass_(
		_mass),
	position_(
		_position),
	transformation_(
		_transformation),
	linear_velocity_(
		_linear_velocity),
	angular_velocity_(
		_angular_velocity),
	lock_duration_(
		_lock_duration),
	timer_callback_(
		_timer_callback)
{
}

sge::renderer::texture::planar_ptr const 
fruitcut::app::fruit::object_parameters::texture() const
{
	return texture_;
}

fruitcut::physics::world &
fruitcut::app::fruit::object_parameters::physics_world() const
{
	return world_;
}

sge::renderer::device &
fruitcut::app::fruit::object_parameters::renderer() const
{
	return renderer_;
}

sge::renderer::vertex_declaration &
fruitcut::app::fruit::object_parameters::vertex_declaration() const
{
	return vertex_declaration_;
}

sge::shader::object &
fruitcut::app::fruit::object_parameters::shader() const
{
	return shader_;
}

fruitcut::app::fruit::mesh const &
fruitcut::app::fruit::object_parameters::mesh() const
{
	return mesh_;
}

fruitcut::physics::scalar 
fruitcut::app::fruit::object_parameters::mass() const
{
	return mass_;
}

fruitcut::physics::vector3 const &
fruitcut::app::fruit::object_parameters::position() const
{
	return position_;
}

fruitcut::physics::matrix4 const &
fruitcut::app::fruit::object_parameters::transformation() const
{
	return transformation_;
}

fruitcut::physics::vector3 const &
fruitcut::app::fruit::object_parameters::linear_velocity() const
{
	return linear_velocity_;
}

fruitcut::physics::vector3 const &
fruitcut::app::fruit::object_parameters::angular_velocity() const
{
	return angular_velocity_;
}

sge::time::duration const &
fruitcut::app::fruit::object_parameters::lock_duration() const
{
	return lock_duration_;
}

sge::time::callback const &
fruitcut::app::fruit::object_parameters::timer_callback() const
{
	return timer_callback_;
}
