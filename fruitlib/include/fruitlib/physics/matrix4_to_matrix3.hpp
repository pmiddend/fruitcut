#ifndef FRUITLIB_PHYSICS_MATRIX4_TO_MATRIX3_HPP_INCLUDED
#define FRUITLIB_PHYSICS_MATRIX4_TO_MATRIX3_HPP_INCLUDED

#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/matrix/static.hpp>


namespace fruitlib
{
namespace physics
{
template<typename T>
fcppt::math::matrix::static_<T,3,3> const
matrix4_to_matrix3(
	fcppt::math::matrix::static_<T,4,4> const &m)
{
	return
		fcppt::math::matrix::static_<T,3,3>(
			m[0][0],m[0][1],m[0][2],
			m[1][0],m[1][1],m[1][2],
			m[2][0],m[2][1],m[2][2]);
}
}
}

#endif
