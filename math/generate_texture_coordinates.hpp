#ifndef FRUITCUT_MATH_GENERATE_TEXTURE_COORDINATES_HPP_INCLUDED
#define FRUITCUT_MATH_GENERATE_TEXTURE_COORDINATES_HPP_INCLUDED

#include "line/distance_to_point.hpp"
#include "line/basic.hpp"
#include <fcppt/assert.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/math/vector/orthogonalize.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/math/range_compare.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/assign/make_container.hpp>
#include <boost/foreach.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp> 
#include <boost/spirit/home/phoenix/core/argument.hpp> 
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/operator/comparison.hpp> 
#include <boost/next_prior.hpp> 
#include <algorithm>
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
	PointContainer const &input)
{
	using namespace boost::phoenix::arg_names;

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
		input.size() >= static_cast<typename PointContainer::size_type>(3));

	scalar const epsilon = 
		static_cast<scalar>(
			0.0001);

	// The first point shouldn't be the second point. Technically no
	// problem, we'd have to look for the first point not being equal to
	// input[0], but I just hope it never happens.
	FCPPT_ASSERT_MESSAGE(
		!fcppt::math::range_compare(
			input[0],
			input[1],
			epsilon),
		FCPPT_TEXT("The first tesselated point is equal to the second, that's not possible right now"));

	// We want to build a parametric equation for the plane in which all
	// points reside. We cannot, however, just take input[0] and build
	// two arrows to input[1] and input[2], respectively, because all
	// three points could be colinear!
	// So we take the arrow from input[0] to input[1] and look for
	// another arrow (to input[i], i > 1) which isn't colinear.
	typename PointContainer::const_iterator const second_point = 
		std::find_if(
			boost::next(
				input.begin(),
				2),
			input.end(),
			boost::phoenix::bind(
				&line::distance_to_point<scalar,3>,
				arg1,
				line::basic<scalar,3>(
					input[0],
					input[1] - input[0])) > epsilon);

	FCPPT_ASSERT_MESSAGE(
		second_point != input.end(),
		FCPPT_TEXT("Degenerate triangle found"));
		
	// Build a plane parameter equation from the first point and the
	// vectors from the first point to the second and third. Then,
	// orthonormalize the direction vectors.
	std::vector<point> const parametrization = 
		fcppt::algorithm::map<std::vector<point> >(
			fcppt::math::vector::orthogonalize(
				fcppt::assign::make_container<std::vector<point> >
					(input[1] - input[0])
					((*second_point) - input[0]).container()),
			&fcppt::math::vector::normalize
			<
				scalar,
				// FIXME: 3 is hard coded here
				boost::mpl::integral_c<fcppt::math::size_type,3>,
				typename fcppt::math::detail::static_storage<scalar,3>::type
			>);

	// That's the parametric equation: a + f * b + g * c with (f,g) as
	// the parameters
	point
		a = input[0],
		b = parametrization[0],
		c = parametrization[1];

	// This scary looking statement assigns (f,g) (the plane parameters)
	// to each point.
	TargetContainer result = 
		fcppt::algorithm::map<TargetContainer>(
			input,
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
	result.clear();
	BOOST_FOREACH(
		point const &x,
		input)
	{
		result.push_back(
			texcoord(
				(a[1]*c[0] - a[0]*c[1] + c[1]*x[0] - c[0]*x[1])/(b[0]*c[1] - b[1]*c[0]),
				(a[1]*b[0] - a[0]*b[1] + b[1]*x[0] - b[0]*x[1])/(b[1]*c[0] - b[0]*c[1])));

		maxdist[0] = std::max(result.back()[0],maxdist[0]);
		maxdist[1] = std::max(result.back()[1],maxdist[1]);
	}

	// And here's the normalization
	BOOST_FOREACH(texcoord &x,result)
		x = texcoord(x[0]/maxdist[0],x[1]/maxdist[1]);

	return result;
}
}
}

#endif
