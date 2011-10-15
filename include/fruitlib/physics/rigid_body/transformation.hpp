#ifndef FRUITLIB_PHYSICS_RIGID_BODY_TRANSFORMATION_HPP_INCLUDED
#define FRUITLIB_PHYSICS_RIGID_BODY_TRANSFORMATION_HPP_INCLUDED

#include <fruitlib/physics/matrix4.hpp>
#include <fcppt/strong_typedef.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>

namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
FCPPT_MAKE_STRONG_TYPEDEF(
	physics::matrix4,
	transformation);
}
}
}

#endif
