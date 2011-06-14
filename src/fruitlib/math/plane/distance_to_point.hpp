#ifndef FRUITCUT_FRUITLIB_MATH_PLANE_DISTANCE_TO_POINT_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_MATH_PLANE_DISTANCE_TO_POINT_HPP_INCLUDED

#include "basic.hpp"
#include "normalize.hpp"

namespace fruitcut
{
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
}

#endif