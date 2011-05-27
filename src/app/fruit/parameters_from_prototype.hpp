#ifndef FRUITCUT_APP_FRUIT_PARAMETERS_FROM_PROTOTYPE_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_PARAMETERS_FROM_PROTOTYPE_HPP_INCLUDED

#include "object_parameters.hpp"
#include "prototype_fwd.hpp"
#include "../../physics/world_fwd.hpp"
#include "../../physics/scalar.hpp"
#include "../../physics/vector3.hpp"
#include "../../physics/matrix4.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>
#include <sge/shader/object_fwd.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/matrix/matrix.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
object_parameters const
parameters_from_prototype(
	fruit::prototype const &proto,
	physics::world &_world,
	sge::renderer::device &_renderer,
	sge::renderer::vertex_declaration &_vertex_declaration,
	sge::shader::object &_shader,
	physics::scalar const _mass,
	physics::vector3 const &_position,
	physics::matrix4 const &_transformation,
	physics::vector3 const &_linear_velocity,
	physics::vector3 const &_angular_velocity);
}
}
}

#endif
