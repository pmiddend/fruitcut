#ifndef FRUITLIB_PHYSICS_VECTOR3_HPP_INCLUDED
#define FRUITLIB_PHYSICS_VECTOR3_HPP_INCLUDED

#include "scalar.hpp"
#include <fcppt/math/vector/vector.hpp>

namespace fruitlib
{
namespace physics
{
typedef
fcppt::math::vector::static_<scalar,3>::type
vector3;
}
}

#endif
