#ifndef FRUITCUT_MATH_CUT_TRIANGLE_AT_PLANE_HPP_INCLUDED
#define FRUITCUT_MATH_CUT_TRIANGLE_AT_PLANE_HPP_INCLUDED

#include "triangle/basic.hpp"
#include "plane/distance_to_point.hpp"
#include "line_plane_intersection.hpp"
#include "triangle_plane_intersection.hpp"
#include <fcppt/math/size_type.hpp>
#include <fcppt/algorithm/map.hpp>
#include <vector>
#include <algorithm>

namespace fruitcut
{
namespace math
{
template
<
	typename T,
	fcppt::math::size_type N,
	typename Data,
	typename Interpolator
>
triangle_plane_intersection<T,N,Data> const
cut_triangle_at_plane(
	triangle::basic<T,N,Data> const &t,
	plane::basic<T,N> const &p,
	Interpolator const &interp)
{
	triangle_plane_intersection<T,N,Data> result;

	typedef
	std::vector<T>
	scalar_sequence;

	typedef typename
	scalar_sequence::size_type
	size_type;

	typedef
	triangle::basic<T,N,Data>
	triangle_type;

	typedef typename
	triangle_type::vector
	vector;

	scalar_sequence const signs = 
		fcppt::algorithm::map<scalar_sequence>(
			t.points(),
			[&p](vector const &v) 
			{
				return plane::distance_to_point(p,v);
			});

	size_type const culled_vertices = 
		static_cast<size_type>(
			std::count_if(
				signs.begin(),
				signs.end(),
				[](T const &t) { return t < static_cast<T>(0); }));

	// Two trivial cases
	// All of the vertices are below the plane => cull everything
	if (culled_vertices == t.points().size())
		return result;

	// None of the vertices are below the plane => cull nothing
	if (culled_vertices == static_cast<size_type>(0))
	{
		result.push_back(
			t);
		return result;
	}

	// So we've got either one or two vertices below the plane. v is the
	// vertex which is "alone", either on the positive or the negative
	// side of the plane
	size_type const 
		v = 
			static_cast<size_type>(
				std::distance(
					signs.begin(),
					std::find_if(
						signs.begin(),
						signs.end(),
						[&culled_vertices](T const &t) 
						{ 
							return 
								culled_vertices == static_cast<size_type>(1)
								?
									t < static_cast<T>(0)
								:
									t >= static_cast<T>(0);
						}))),
		vnext = 
			static_cast<size_type>(v+1) % signs.size(),
		vprev = 
			v == static_cast<size_type>(0)
			?
				static_cast<size_type>(signs.size()-1)
			:
				static_cast<size_type>(v-1);

	vector const
		s_1 = 
			*line_plane_intersection<T,N>(
				line::basic<T,N>(
					t.points()[vprev],
					t.points()[v] - t.points()[vprev]),
				p),
		s_2 = 
			*line_plane_intersection<T,N>(
				line::basic<T,N>(
					t.points()[vnext],
					t.points()[v] - t.points()[vnext]),
				p);

	Data const
		s_1_data = 
			interp(
				t.data()[vprev],
				t.data()[v],
				fcppt::math::vector::length(
					s_1-t.points()[vprev])/
				fcppt::math::vector::length(
					t.points()[vprev]-t.points()[v])),
		s_2_data = 
			interp(
				t.data()[vnext],
				t.data()[v],
				fcppt::math::vector::length(
					s_2-t.points()[vnext])/
				fcppt::math::vector::length(
					t.points()[vnext]-t.points()[v]));

	result.push_back(
		s_1);
	result.push_back(
		s_2);

	if (culled_vertices == static_cast<size_type>(2))
	{
		result.push_back(
			triangle_type(
				{
					s_1,
					t.points()[v],
					s_2
				},
				{
					s_1_data,
					t.data()[v],
					s_2_data
				}));
	}
	else
	{
		result.push_back(
			triangle_type(
				{
					t.points()[vprev],
					s_1,
					t.points()[vnext]
				},
				{
					t.data()[vprev],
					s_1_data,
					t.data()[vnext]
				}));

		result.push_back(
			triangle_type(
				{
					s_1,
					s_2,
					t.points()[vnext]
				},
				{
					s_1_data,
					s_2_data,
					t.data()[vnext]
				}));
	}
	
	return result;
}
}
}

#endif
