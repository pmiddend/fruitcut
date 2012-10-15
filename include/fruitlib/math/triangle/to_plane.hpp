#ifndef FRUITLIB_MATH_TRIANGLE_TO_PLANE_HPP_INCLUDED
#define FRUITLIB_MATH_TRIANGLE_TO_PLANE_HPP_INCLUDED

#include <fruitlib/math/plane/basic.hpp>
#include <fruitlib/math/triangle/basic.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/dot.hpp>
#include <fcppt/math/vector/normalize.hpp>


namespace fruitlib
{
namespace math
{
namespace triangle
{
template<typename T>
plane::basic<T> const
to_plane(
	basic<T,3> const &t)
{
	typename basic<T,3>::vector normal =
		fcppt::math::vector::normalize(
			fcppt::math::vector::cross(
				t.points[1] - t.points[0],
				t.points[2] - t.points[0]));
	return
		plane::basic<T>(
			normal,
			fcppt::math::vector::dot(
				normal,
				t.points[0]));
}
}
}
}

#endif