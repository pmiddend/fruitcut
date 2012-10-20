#ifndef FRUITLIB_MATH_CUT_TRIANGLE_AT_PLANE_HPP_INCLUDED
#define FRUITLIB_MATH_CUT_TRIANGLE_AT_PLANE_HPP_INCLUDED

#include <fruitlib/math/line_plane_intersection.hpp>
#include <fruitlib/math/triangle_plane_intersection.hpp>
#include <fruitlib/math/plane/distance_to_point.hpp>
#include <fruitlib/math/triangle/create_from_interpolation.hpp>
#include <fruitlib/math/triangle/interpolation_pair.hpp>
#include <fruitlib/math/triangle/scalar_type.hpp>
#include <fruitlib/math/triangle/vector_type.hpp>
#include <fruitlib/math/triangle/vertex_access.hpp>
#include <fcppt/static_assert_expression.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/math/range_compare.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/operator/comparison.hpp>
#include <boost/spirit/home/phoenix/operator/if_else.hpp>
#include <boost/type_traits/is_same.hpp>
#include <algorithm>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace math
{
template
<
	typename Triangle,
	typename Plane,
	typename EpsilonType
>
triangle_plane_intersection<Triangle> const
cut_triangle_at_plane(
	Triangle const &t,
	Plane const &p,
	EpsilonType const epsilon)
{
	typedef
	Triangle
	triangle_type;

	typedef typename
	triangle::vector_type<triangle_type>::type
	vector;

	typedef typename
	triangle::scalar_type<triangle_type>::type
	scalar;

	FCPPT_STATIC_ASSERT_EXPRESSION((
		boost::is_same<EpsilonType,scalar>::value));

	typedef
	triangle_plane_intersection<triangle_type>
	result_type;

	typedef
	fcppt::container::array<scalar,3>
	scalar_sequence;

	typedef typename
	scalar_sequence::size_type
	size_type;

	typename vector::size_type const N =
		vector::dim_wrapper::value;

	result_type result;

	fcppt::container::array<vector,3> const points =
	{{
		triangle::vertex_access<triangle_type>::get(t,0),
		triangle::vertex_access<triangle_type>::get(t,1),
		triangle::vertex_access<triangle_type>::get(t,2)
	}};

	scalar_sequence signs;

	for(std::size_t i = 0; i < 3; ++i)
		signs[i] =
			plane::distance_to_point(
				p,
				points[i]);

	size_type const culled_vertices =
		static_cast<size_type>(
			std::count_if(
				signs.begin(),
				signs.end(),
				boost::phoenix::arg_names::arg1 < static_cast<scalar>(0)));

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
						boost::phoenix::if_else(
							boost::phoenix::val(
								culled_vertices == static_cast<size_type>(1)),
							boost::phoenix::arg_names::arg1 < static_cast<scalar>(0),
							boost::phoenix::arg_names::arg1 >= static_cast<scalar>(0))))),
		vnext =
			static_cast<size_type>(v+1) % signs.size(),
		vprev =
			v == static_cast<size_type>(0)
			?
				static_cast<size_type>(signs.size()-1)
			:
				static_cast<size_type>(v-1);

	// If the line through one of the triangle edges is too small, the
	// line::basic constructor will fail. But then again, when that
	// happens, we know that our triangle is extremely small, so we
	// might as well cull it.
	if(fcppt::math::range_compare(points[v],points[vprev],epsilon) || fcppt::math::range_compare(points[v],points[vnext],epsilon))
		return result;

	typedef
	fcppt::optional<vector>
	optional_vector;

	optional_vector const
		s_1 =
			line_plane_intersection<scalar,N>(
				line::basic<scalar,N>(
					points[vprev],
					points[v] - points[vprev]),
				p,
				epsilon),
		s_2 =
			line_plane_intersection<scalar,N>(
				line::basic<scalar,N>(
					points[vnext],
					points[v] - points[vnext]),
				p,
				epsilon);

	// I have no clue when this is supposed to happen, but if it does, reject it!
	if(!s_1 || !s_2)
		return result;

	typedef
	triangle::interpolation_pair<triangle_type>
	interpolation_pair_type;

	interpolation_pair_type const
		v_pair(
			v,
			v,
			static_cast<scalar>(
				0)),
		vnext_pair(
			vnext,
			vnext,
			static_cast<scalar>(
				0)),
		vprev_pair(
			vprev,
			vprev,
			static_cast<scalar>(
				0)),
		s_1_pair(
			v,
			vprev,
			fcppt::math::vector::length(
				(*s_1)-points[vprev])/
			fcppt::math::vector::length(
				points[vprev]-points[v])),
		s_2_pair(
			v,
			vnext,
			fcppt::math::vector::length(
				(*s_2)-points[vnext])/
			fcppt::math::vector::length(
				points[vnext]-points[v]));

	result.push_back(
		*s_1);
	result.push_back(
		*s_2);

	if (culled_vertices == static_cast<size_type>(2))
	{
		result.push_back(
			triangle::create_from_interpolation<triangle_type>::create(
				t,
				s_1_pair,
				v_pair,
				s_2_pair));
	}
	else
	{
		result.push_back(
			triangle::create_from_interpolation<triangle_type>::create(
				t,
				vprev_pair,
				s_1_pair,
				vnext_pair));

		result.push_back(
			triangle::create_from_interpolation<triangle_type>::create(
				t,
				s_1_pair,
				s_2_pair,
				vnext_pair));
	}

	return result;
}
}
}

#endif
