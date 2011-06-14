#ifndef FRUITCUT_APP_FRUIT_PARAMETERS_FROM_PROTOTYPE_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_PARAMETERS_FROM_PROTOTYPE_HPP_INCLUDED

#include "object_parameters.hpp"
#include "prototype_fwd.hpp"
#include "../../fruitlib/physics/world_fwd.hpp"
#include "../../fruitlib/physics/scalar.hpp"
#include "../../fruitlib/physics/vector3.hpp"
#include "../../fruitlib/physics/matrix4.hpp"
#include "../../fruitlib/physics/group/object_fwd.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>
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
	fruitlib::physics::world &_world,
	fruitlib::physics::group::object &_fruit_group,
	sge::renderer::device &_renderer,
	sge::renderer::vertex_declaration &_vertex_declaration,
	fruitlib::physics::scalar const _mass,
	fruitlib::physics::vector3 const &_position,
	fruitlib::physics::matrix4 const &_transformation,
	fruitlib::physics::vector3 const &_linear_velocity,
	fruitlib::physics::vector3 const &_angular_velocity);
}
}
}

#endif