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
template<typename T,typename Data>
fruitlib::math::plane::basic<T,3> const
to_plane(
	fruitlib::math::triangle::basic<T,3,Data> const &t)
{
	typename fruitlib::math::triangle::basic<T,3,Data>::vector normal =
		fcppt::math::vector::normalize(
			fcppt::math::vector::cross(
				t.points[1] - t.points[0],
				t.points[2] - t.points[0]));
	return
		fruitlib::math::plane::basic<T,3>(
			normal,
			fcppt::math::vector::dot(
				normal,
				t.points[0]));
}
}
}
}

#endif
