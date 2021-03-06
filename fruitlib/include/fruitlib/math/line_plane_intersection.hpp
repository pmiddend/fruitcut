#ifndef FRUITLIB_MATH_LINE_PLANE_INTERSECTION_HPP_INCLUDED
#define FRUITLIB_MATH_LINE_PLANE_INTERSECTION_HPP_INCLUDED

#include <fruitlib/math/line/basic.hpp>
#include <fruitlib/math/plane/basic.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/math/vector/dot.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/config/external_begin.hpp>
#include <cmath>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace math
{
// We're looking for the point p which is on the plane and the
// line. The plane is defined by:
// dot(n,p) = lambda
// And the line is defined by:
// a + nu * b = p
// Let's substitute p in the plane equation by the line equation:
// dot(n,a + nu * b) = lambda
// => nu = (lambda - dot(n,a))/dot(n,b)
template<typename T,fcppt::math::size_type N>
fcppt::optional::object<typename line::basic<T,N>::vector> const
line_plane_intersection(
	line::basic<T,N> const &l,
	plane::basic<T,N> const &p,
	T const epsilon)
{
	// Normal of the plane and direction of the line are perpendicular:
	// no intersection
	if (std::abs(fcppt::math::vector::dot(p.normal(),l.direction())) < epsilon)
		return fcppt::optional::object<typename line::basic<T,N>::vector>();

	return
		fcppt::optional::object<typename line::basic<T,N>::vector>(
			l.point()
				+ (p.lambda() - fcppt::math::vector::dot(p.normal(),l.point()))
				/ fcppt::math::vector::dot(p.normal(),l.direction())
				* l.direction());
}
}
}

#endif
