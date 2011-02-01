#ifndef FRUITCUT_PHYSICS_MATRIX4_HPP_INCLUDED
#define FRUITCUT_PHYSICS_MATRIX4_HPP_INCLUDED

#include "scalar.hpp"
#include <fcppt/math/matrix/static.hpp>

namespace fruitcut
{
namespace physics
{
typedef
fcppt::math::matrix::static_
<
	scalar,
	4,
	4
>::type
matrix4;
}
}

#endif
