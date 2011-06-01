#ifndef FRUITCUT_FRUITLIB_PHYSICS_MATRIX3_TO_MATRIX4_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_MATRIX3_TO_MATRIX4_HPP_INCLUDED

#include <fcppt/math/matrix/static.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace physics
{
template<typename T>
typename
fcppt::math::matrix::static_<T,4,4>::type const
matrix3_to_matrix4(
	typename
	fcppt::math::matrix::static_<T,3,3>::type const &m)
{
	return 
		typename
		fcppt::math::matrix::static_<T,4,4>::type(
			m[0][0],m[0][1],m[0][2],static_cast<T>(0),
			m[1][0],m[1][1],m[1][2],static_cast<T>(0),
			m[2][0],m[2][1],m[2][2],static_cast<T>(0),
			static_cast<T>(0),static_cast<T>(0),static_cast<T>(0),static_cast<T>(1));
}
}
}
}

#endif
