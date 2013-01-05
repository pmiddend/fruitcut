#include <fruitapp/fruit/object.hpp>
#include <fruitapp/fruit/object_from_prototype.hpp>
#include <fruitapp/fruit/prototype.hpp>
#include <fcppt/make_unique_ptr.hpp>


fruitapp::fruit::object_unique_ptr
fruitapp::fruit::object_from_prototype(
	fruit::prototype const &_prototype,
	fruitlib::physics::world &_physics_world,
	fruitlib::physics::group::object &_fruit_group,
	sge::renderer::device::core &_renderer,
	sge::renderer::vertex::declaration &_vertex_declaration,
	fruitlib::physics::rigid_body::mass const _mass,
	fruitlib::physics::rigid_body::position const &_position,
	fruitlib::physics::rigid_body::transformation const &_transformation,
	fruitlib::physics::rigid_body::linear_velocity const &_linear_velocity,
	fruitlib::physics::rigid_body::angular_velocity const &_angular_velocity,
	fruitapp::ingame_clock const &_clock)
{
	return
		fcppt::make_unique_ptr<fruitapp::fruit::object>(
			_prototype,
			_physics_world,
			_renderer,
			_vertex_declaration,
			fcppt::make_unique_ptr<fruitapp::fruit::mesh>(
				_prototype.mesh().triangles()),
			_fruit_group,
			_mass,
			_position,
			_transformation,
			_linear_velocity,
			_angular_velocity,
			fruitapp::fruit::ban_duration(
				fruitapp::ingame_clock::duration()),
			_clock);
}
