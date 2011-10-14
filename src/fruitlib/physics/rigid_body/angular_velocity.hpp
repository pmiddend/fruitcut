#ifndef FRUITLIB_PHYSICS_RIGID_BODY_ANGULAR_VELOCITY_HPP_INCLUDED
#define FRUITLIB_PHYSICS_RIGID_BODY_ANGULAR_VELOCITY_HPP_INCLUDED

#include <fruitlib/physics/vector3.hpp>
#include <fcppt/strong_typedef.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
FCPPT_MAKE_STRONG_TYPEDEF(
	physics::vector3,
	angular_velocity);
}
}
}

#endif
