#ifndef FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_LINEAR_VELOCITY_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_RIGID_BODY_LINEAR_VELOCITY_HPP_INCLUDED

#include "../vector3.hpp"
#include <fcppt/strong_typedef.hpp>
#include <fcppt/math/vector/vector.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
FCPPT_MAKE_STRONG_TYPEDEF(
	physics::vector3,
	linear_velocity);
}
}
}
}

#endif
