#ifndef FRUITLIB_PHYSICS_MATRIX4_HPP_INCLUDED
#define FRUITLIB_PHYSICS_MATRIX4_HPP_INCLUDED

#include <fruitlib/physics/scalar.hpp>
#include <fcppt/math/matrix/static.hpp>


namespace fruitlib
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
