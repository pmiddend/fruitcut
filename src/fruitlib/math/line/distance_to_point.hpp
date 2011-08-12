#ifndef FRUITLIB_MATH_LINE_DISTANCE_TO_POINT_HPP_INCLUDED
#define FRUITLIB_MATH_LINE_DISTANCE_TO_POINT_HPP_INCLUDED

#include <fruitlib/math/line/basic.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/dot.hpp>
#include <fcppt/math/size_type.hpp>

namespace fruitlib
{
namespace math
{
namespace line
{
template<typename T,fcppt::math::size_type N>
T
distance_to_point(
	typename basic<T,N>::vector const &p,
	basic<T,N> const &l)
{
	return
		fcppt::math::vector::length(
			p -
			l.point() -
			(
				fcppt::math::vector::dot(
					l.direction(),
					p - l.point()) /
				fcppt::math::vector::dot(
					l.direction(),
					l.direction()))
			* l.direction());
}
}
}
}

#endif
