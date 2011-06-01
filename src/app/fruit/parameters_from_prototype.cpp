#include "parameters_from_prototype.hpp"
#include "prototype.hpp"
#include "../../fruitlib/physics/world_fwd.hpp"
#include "../../fruitlib/physics/scalar.hpp"
#include "../../fruitlib/physics/vector3.hpp"
#include "../../fruitlib/physics/matrix4.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>
#include <sge/shader/object_fwd.hpp>
#include <sge/time/default_callback.hpp>
#include <sge/time/duration.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/chrono/duration.hpp>

fruitcut::app::fruit::object_parameters const
fruitcut::app::fruit::parameters_from_prototype(
	fruit::prototype const &proto,
	fruitlib::physics::world &_world,
	sge::renderer::device &_renderer,
	sge::renderer::vertex_declaration &_vertex_declaration,
	sge::shader::object &_shader,
	fruitlib::physics::scalar const _mass,
	fruitlib::physics::vector3 const &_position,
	fruitlib::physics::matrix4 const &_transformation,
	fruitlib::physics::vector3 const &_linear_velocity,
	fruitlib::physics::vector3 const &_angular_velocity)
{
	return 
		fruit::object_parameters(
			proto.texture(),
			proto.splatter_color(),
			_world,
			_renderer,
			_vertex_declaration,
			_shader,
			proto.mesh(),
			_mass,
			_position,
			_transformation,
			_linear_velocity,
			_angular_velocity,
			sge::time::duration(),
			sge::time::default_callback());
}
