#ifndef FRUITCUT_PHYSICS_MATRIX3_HPP_INCLUDED
#define FRUITCUT_PHYSICS_MATRIX3_HPP_INCLUDED

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
	3,
	3
>::type
matrix3;
}
}

#endif
