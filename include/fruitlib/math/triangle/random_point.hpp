#ifndef FRUITLIB_MATH_TRIANGLE_RANDOM_POINT_HPP_INCLUDED
#define FRUITLIB_MATH_TRIANGLE_RANDOM_POINT_HPP_INCLUDED

#include <fruitlib/math/triangle/scalar_type.hpp>
#include <fruitlib/math/triangle/vector_type.hpp>
#include <fruitlib/math/triangle/vertex_access.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <algorithm>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace math
{
namespace triangle
{
// Taken from http://www.cgafaq.info/wiki/Random_Point_In_Triangle
template<typename Triangle,typename Rng>
typename
triangle::vector_type<Triangle>::type const
random_point(
	Triangle const &t,
	Rng &r)
{
	/*
	typedef typename
	triangle::vector_type<Triangle>::type
	vector;
	*/

	typedef typename
	triangle::scalar_type<Triangle>::type
	scalar;

	scalar const
		r1 =
			r(),
		r2 =
			r(),
		a1 =
			r1 + r2 > 1
			?
				1-r1
			:
				r1,
		a2 =
			r1 + r2 > 1
			?
				1-r2
			:
				r2,
		a3 =
			1-a1-a2;

	return
		a1 * triangle::vertex_access<Triangle>::get(t,0) +
		a2 * triangle::vertex_access<Triangle>::get(t,1) +
		a3 * triangle::vertex_access<Triangle>::get(t,2);
}
}
}
}

#endif
