#include <fruitapp/fruit/object.hpp>
#include <fruitapp/fruit/object_from_prototype.hpp>
#include <fruitapp/fruit/prototype.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>


fruitapp::fruit::object_unique_ptr
fruitapp::fruit::object_from_prototype(
	fruit::prototype const &_prototype,
	fruitlib::physics::world &_physics_world,
	fruitlib::physics::group::object &_fruit_group,
	sge::renderer::device::core &_renderer,
	sge::renderer::vertex_declaration &_vertex_declaration,
	fruitlib::physics::scalar const _mass,
	fruitlib::physics::vector3 const &_position,
	fruitlib::physics::matrix4 const &_transformation,
	fruitlib::physics::vector3 const &_linear_velocity,
	fruitlib::physics::vector3 const &_angular_velocity,
	fruitapp::ingame_clock const &_clock)
{
	return
		fcppt::make_unique_ptr<fruitapp::fruit::object>(
			fcppt::cref(
				_prototype),
			fcppt::ref(
				_physics_world),
			fcppt::ref(
				_renderer),
			fcppt::ref(
				_vertex_declaration),
			fcppt::make_unique_ptr<fruitapp::fruit::mesh>(
				fcppt::cref(
					_prototype.mesh().triangles())),
			fcppt::ref(
				_fruit_group),
			_mass,
			_position,
			_transformation,
			_linear_velocity,
			_angular_velocity,
			fruitapp::ingame_clock::duration(),
			fcppt::cref(
				_clock));
}
