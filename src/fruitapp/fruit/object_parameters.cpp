#include <fruitapp/fruit/object_parameters.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>

fruitapp::fruit::object_parameters::object_parameters(
	fruit::prototype const &_prototype,
	fruitlib::physics::world &_world,
	sge::renderer::device &_renderer,
	sge::renderer::vertex_declaration &_vertex_declaration,
	fruitapp::fruit::mesh const &_mesh,
	fruitlib::physics::group::object &_fruit_group,
	fruitlib::physics::scalar const _mass,
	fruitlib::physics::vector3 const &_position,
	fruitlib::physics::matrix4 const &_transformation,
	fruitlib::physics::vector3 const &_linear_velocity,
	fruitlib::physics::vector3 const &_angular_velocity,
	fruitapp::ingame_clock::duration const &_lock_duration,
	fruitapp::ingame_clock const &_clock)
:
	prototype_(
		_prototype),
	world_(
		_world),
	renderer_(
		_renderer),
	vertex_declaration_(
		_vertex_declaration),
	mesh_(
		_mesh),
	fruit_group_(
		_fruit_group),
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
	clock_(
		_clock)
{
}

fruitapp::fruit::prototype const &
fruitapp::fruit::object_parameters::prototype() const
{
	return prototype_;
}

fruitlib::physics::world &
fruitapp::fruit::object_parameters::physics_world() const
{
	return world_;
}

sge::renderer::device &
fruitapp::fruit::object_parameters::renderer() const
{
	return renderer_;
}

sge::renderer::vertex_declaration &
fruitapp::fruit::object_parameters::vertex_declaration() const
{
	return vertex_declaration_;
}

fruitapp::fruit::mesh const &
fruitapp::fruit::object_parameters::mesh() const
{
	return mesh_;
}

fruitlib::physics::group::object & 
fruitapp::fruit::object_parameters::fruit_group() const
{
	return fruit_group_;
}

fruitlib::physics::scalar 
fruitapp::fruit::object_parameters::mass() const
{
	return mass_;
}

fruitlib::physics::vector3 const &
fruitapp::fruit::object_parameters::position() const
{
	return position_;
}

fruitlib::physics::matrix4 const &
fruitapp::fruit::object_parameters::transformation() const
{
	return transformation_;
}

fruitlib::physics::vector3 const &
fruitapp::fruit::object_parameters::linear_velocity() const
{
	return linear_velocity_;
}

fruitlib::physics::vector3 const &
fruitapp::fruit::object_parameters::angular_velocity() const
{
	return angular_velocity_;
}

fruitapp::ingame_clock::duration const &
fruitapp::fruit::object_parameters::lock_duration() const
{
	return lock_duration_;
}

fruitapp::ingame_clock const &
fruitapp::fruit::object_parameters::clock() const
{
	return clock_;
}