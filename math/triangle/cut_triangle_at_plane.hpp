#ifndef FRUITCUT_MATH_TRIANGLE_CUT_TRIANGLE_AT_PLANE_HPP_INCLUDED
#define FRUITCUT_MATH_TRIANGLE_CUT_TRIANGLE_AT_PLANE_HPP_INCLUDED

#include "triangle/basic.hpp"
#include "plane/distance_to_point.hpp"
#include "triangle_plane_intersection.hpp"
#include <fcppt/math/size_type.hpp>
#include <fcppt/algorithm/map.hpp>
#include <vector>
#include <algorithm>
#include <functional>
#include <tuple>


namespace fruitcut
{
namespace math
{
namespace triangle
{
template<typename T,fcppt::math::size_type N>
triangle_plane_intersection const
cut_triangle_at_plane(
	triangle::basic<T,N> const &t,
	plane::basic<T,N> const &p)
{
	triangle_plane_intersection<T,N> result;

	typedef
	std::vector<T>
	scalar_container;

	scalar_container const distances = 
		fcppt::algorithm::map<scalar_container>(
			t.points,
			std::bind(
				&plane::distance_to_point,
				p,
				std::placeholders::_1));
	
	// Simple cases: Triangle is completely above or below the plane
	if(
		std::all_of(
			distances.begin(),
			distances.end(),
			[](T const v) 
			{
				return 
					 v > static_cast<T>(0);
			}))
	{
		result.insert(
			t);
		return result;
	}

	// Below
	if(
		std::all_of(
			t.points.begin(),
			t.points.end(),
			[](typename triangle_type::vector const &v) 
			{
				return 
					v < static_cast<T>(0);
			}))
		return result;

	// Ok, now the nontrivial case: The triangle intersects the plane
	typename triangle_plane_intersection<T,N>::triangle_type::point_sequence points;
	
}
}
}
}

#endif
