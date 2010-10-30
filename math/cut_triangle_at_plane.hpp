#ifndef FRUITCUT_MATH_CUT_TRIANGLE_AT_PLANE_HPP_INCLUDED
#define FRUITCUT_MATH_CUT_TRIANGLE_AT_PLANE_HPP_INCLUDED

#include "triangle/basic.hpp"
#include "plane/distance_to_point.hpp"
#include "line_plane_intersection.hpp"
#include "triangle_plane_intersection.hpp"
#include <fcppt/math/size_type.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/io/cout.hpp>
#include <vector>
#include <algorithm>

namespace fruitcut
{
namespace math
{
template
<
	typename Triangle,
	typename Plane,
	typename GetPosition,
	typename GetData,
	typename InterpolateVertex,
	typename CreateTriangle
>
triangle_plane_intersection<Triangle> const
cut_triangle_at_plane(
	Triangle const &t,
	Plane const &p,
	GetPosition const &position,
	GetData const &data,
	InterpolateVertex const &interpolate,
	CreateTriangle const &create_triangle)
{
	typedef
	Triangle
	triangle_type;

	typedef typename
	triangle_type::vector
	vector;

	typedef typename
	vector::value_type
	scalar;

	typedef
	triangle_plane_intersection<triangle_type> 
	result_type;

	typedef
	std::vector<scalar>
	scalar_sequence;

	typedef typename
	scalar_sequence::size_type
	size_type;

	typename vector::size_type const N = vector::dim_wrapper::value;

	result_type result;

	std::array<vector,3> const points = 
	{{
		position(t,0),
		position(t,1),
		position(t,2)
	}};

	std::array<vector,3> const datas = 
	{{
		data(t,0),
		data(t,1),
		data(t,2)
	}};

	scalar_sequence const signs = 
		fcppt::algorithm::map<scalar_sequence>(
			points,
			[&p](vector const &v) { return plane::distance_to_point(p,v); });

	size_type const culled_vertices = 
		static_cast<size_type>(
			std::count_if(
				signs.begin(),
				signs.end(),
				[](scalar const &t) { return t < static_cast<scalar>(0); }));

	// Two trivial cases
	// All of the vertices are below the plane => cull everything
	if (culled_vertices == static_cast<size_type>(3))
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
						[&culled_vertices](scalar const &t) 
						{ 
							return 
								culled_vertices == static_cast<size_type>(1)
								?
									t < static_cast<scalar>(0)
								:
									t >= static_cast<scalar>(0);
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
			*line_plane_intersection<scalar,N>(
				line::basic<scalar,N>(
					points[vprev],
					points[v] - points[vprev]),
				p),
		s_2 = 
			*line_plane_intersection<scalar,N>(
				line::basic<scalar,N>(
					points[vnext],
					points[v] - points[vnext]),
				p);

	typename triangle_type::value_type const
		s_1_data = 
			interpolate(
				datas[v],
				datas[vprev],
				fcppt::math::vector::length(
					s_1-points[vprev])/
				fcppt::math::vector::length(
					points[vprev]-points[v])),
		s_2_data = 
			interpolate(
				datas[v],
				datas[vnext],
				fcppt::math::vector::length(
					s_2-points[vnext])/
				fcppt::math::vector::length(
					points[vnext]-points[v]));

	result.push_back(
		s_1);
	result.push_back(
		s_2);

	if (culled_vertices == static_cast<size_type>(2))
	{
		result.push_back(
			create_triangle(
				s_1_data,
				datas[v],
				s_2_data));
	}
	else
	{
		result.push_back(
			create_triangle(
				datas[vprev],
				s_1_data,
				datas[vnext]));

		result.push_back(
			create_triangle(
					s_1_data,
					s_2_data,
					datas[vnext]));
	}
	
	return result;
}
}
}

#endif
