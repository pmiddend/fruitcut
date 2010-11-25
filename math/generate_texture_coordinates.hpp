#ifndef FRUITCUT_MATH_GENERATE_TEXTURE_COORDINATES_HPP_INCLUDED
#define FRUITCUT_MATH_GENERATE_TEXTURE_COORDINATES_HPP_INCLUDED

#include <fcppt/assert.hpp>
#include <fcppt/math/vector/orthogonalize.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/assign/make_container.hpp>
#include <boost/foreach.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp> 
#include <boost/spirit/home/phoenix/core/argument.hpp> 
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <vector>

namespace fruitcut
{
namespace math
{
/**
	This function gets an arbitrary two or three-dimensional point
	sequence with at least 3 elements as its input and outputs a
	sequence of two-dimensional point sequence of the same size.

	The result contains only points in [0,1] x [0,1]
 */
template<typename TargetContainer,typename PointContainer>
TargetContainer const
generate_texture_coordinates(
	Container const &c)
{
	typedef typename
	PointContainer::value_type
	point;

	typedef typename
	TargetContainer::value_type
	texcoord;

	typedef typename
	point::value_type
	scalar;

	FCPPT_ASSERT(
		c.size() >= static_cast<typename PointContainer::size_type>(3));

	// Build a plane parameter equation from the first point and the
	// vectors from the first point to the second and third. Then,
	// orthonormalize the direction vectors.
	std::vector<point> const parametrization = 
		fcppt::algorithm::map<std::vector<point> >(
			fcppt::math::vector::orthogonalize(
				fcppt::assign::make_container<std::vector<point> >
					(c[1] - c[0])
					(c[2] - c[0])),
			&fcppt::math::vector::normalize<point>);

	// That's the parametric equation: a + f * b + g * c with (f,g) as
	// the parameters
	point
		a = c[0];
		b = parametrization[0],
		c = parametrization[1];

	using boost::phoenix::arg_names;

	// This scary looking statement assigns (f,g) (the plane parameters)
	// to each point.
	TargetContainer result = 
		fcppt::algorithm::map<TargetContainer>(
			c,
			boost::phoenix::construct<texcoord>(
				(a[1]*c[0] - a[0]*c[1] + c[1]*arg1[0] - c[0]*arg1[1])/(b[0]*c[1] - b[1]*c[0]),
				(a[1]*b[0] - a[0]*b[1] + b[1]*arg1[0] - b[0]*arg1[1])/(b[1]*c[0] - b[0]*c[1])));

	// We're interested in the "bounding rectangle" of the points in the
	// plane, so we take the minimum texture coordinates to determine
	// the origin of the bounding rect
	texcoord min = result.front();
	BOOST_FOREACH(texcoord const &t,result)
	{
		min[0] = std::min(min[0],t[0]);
		min[1] = std::min(min[1],t[1]);
	}

	// That's the new origin
	a = a + min[0] * b + min[1] * c;	

	// To normalize the values to [0,1], we also need to know the
	// maximum distance from the origin to a point
	texcoord maxdist = texcoord::null();

	// This calculates the new (f,g) values (for the new origin) and
	// also calculates the above maximum distance
	BOOST_FOREACH(texcoord &x,result)
	{
		x = 
			texcoord(
				(a[1]*c[0] - a[0]*c[1] + c[1]*x[0] - c[0]*x[1])/(b[0]*c[1] - b[1]*c[0]),
				(a[1]*b[0] - a[0]*b[1] + b[1]*x[0] - b[0]*x[1])/(b[1]*c[0] - b[0]*c[1]));

		maxdist[0] = std::max(x[0],maxdist[0]);
		maxdist[1] = std::max(x[1],maxdist[1]);
	}

	// And here's the normalization
	BOOST_FOREACH(texcoord &x,result)
		x = texcoord(x[0]/maxdist[0],x[1]/maxdist[1]);

	return result;
}
}
}

#endif
