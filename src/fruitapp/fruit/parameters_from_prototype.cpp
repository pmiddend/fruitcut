#include <fruitapp/fruit/parameters_from_prototype.hpp>
#include <fruitapp/fruit/prototype.hpp>
#include <fruitlib/physics/matrix4.hpp>
#include <fruitlib/physics/scalar.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>
#include <fcppt/chrono/duration.hpp>


fruitapp::fruit::object_parameters const
fruitapp::fruit::parameters_from_prototype(
	fruit::prototype const &proto,
	fruitlib::physics::world &_world,
	fruitlib::physics::group::object &_fruit_group,
	sge::renderer::device &_renderer,
	sge::renderer::vertex_declaration &_vertex_declaration,
	fruitlib::physics::scalar const _mass,
	fruitlib::physics::vector3 const &_position,
	fruitlib::physics::matrix4 const &_transformation,
	fruitlib::physics::vector3 const &_linear_velocity,
	fruitlib::physics::vector3 const &_angular_velocity,
	fruitapp::ingame_clock const &_clock)
{
	return
		fruit::object_parameters(
			proto,
			_world,
			_renderer,
			_vertex_declaration,
			proto.mesh(),
			_fruit_group,
			_mass,
			_position,
			_transformation,
			_linear_velocity,
			_angular_velocity,
			fruitapp::ingame_clock::duration(),
			_clock);
}
