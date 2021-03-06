#ifndef FRUITLIB_MATH_PLANE_DISTANCE_TO_POINT_HPP_INCLUDED
#define FRUITLIB_MATH_PLANE_DISTANCE_TO_POINT_HPP_INCLUDED

#include <fruitlib/math/plane/basic.hpp>
#include <fruitlib/math/plane/normalize.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/math/vector/dot.hpp>


namespace fruitlib
{
namespace math
{
namespace plane
{
template<typename T,fcppt::math::size_type N>
T const
distance_to_point(
	basic<T,N> const &p,
	typename basic<T,N>::vector const &d)
{
	basic<T,N> const normalized =
		plane::normalize(
			p);

	return
		fcppt::math::vector::dot(
			p.normal(),
			d)
			- p.lambda();
}
}
}
}

#endif
