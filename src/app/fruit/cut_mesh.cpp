#include "cut_mesh.hpp"
#include "cut_mesh_result.hpp"
#include "../../math/cut_triangle_at_plane.hpp"
#include "../../math/triangle_plane_intersection.hpp"
#include "../../math/line/distance_to_point.hpp"
#include "../../geometry_traits/vector.hpp"
#include "../../geometry_traits/box.hpp"
#include "triangle.hpp"
#include "triangle_traits.hpp"
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vector4.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/move.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/box/box.hpp>
#include <fcppt/math/range_compare.hpp>
#include <fcppt/assign/make_array.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <boost/next_prior.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/core.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <boost/geometry/multi/algorithms/detail/for_each_range.hpp>
#include <algorithm>
#include <vector>
#include <set>
#include <iterator>
#include <iostream>
#include <cmath>

namespace
{
sge::renderer::vector2 const
transform_texcoord(
	sge::renderer::vector2 const &t)
{
	return 
		sge::renderer::vector2(
			t.x()/
				static_cast<sge::renderer::scalar>(2) + 
				static_cast<sge::renderer::scalar>(0.5),
			t.y());
}

sge::renderer::scalar
triangle_area(
	fruitcut::app::fruit::triangle const &t)
{
	using namespace fruitcut::app::fruit;
	triangle::vector const 
		ab =
			t.vertices[1] - t.vertices[0],
		ac = 
			t.vertices[2] - t.vertices[0];

	return 
		static_cast<sge::renderer::scalar>(
			0.5) * 
		std::abs(
			ab.x() * ac.y() - ac.x() * ab.y());
}

template<typename Container>
fcppt::optional
<
	typename 
	fcppt::math::matrix::static_
	<
		typename boost::mpl::identity<typename Container::value_type>::type::value_type,
		4,
		4
	>::type
> const
make_coordinate_system(
	Container const &points,
	typename boost::mpl::identity<typename Container::value_type>::type::value_type const epsilon)
{
	typedef typename
	Container::value_type
	vector;

	typedef typename
	vector::value_type
	scalar;

	typedef typename
	Container::size_type
	size_type;

	typedef typename
	Container::const_iterator
	const_iterator;

	typedef typename 
	fcppt::math::matrix::static_<scalar,4,4>::type
	matrix4;

	FCPPT_ASSERT(
		!points.empty());

	const_iterator first_other_point = 
		boost::next(
			points.begin());

	for(; first_other_point != points.end(); ++first_other_point)
		if(
			!fcppt::math::range_compare(
				*points.begin(),
				*first_other_point,
				epsilon))
			break;

	// All the points are equal
	if (first_other_point == points.end())
		return fcppt::optional<matrix4>();

	const_iterator second_other_point = 
		boost::next(
			first_other_point);

	for(; second_other_point != points.end(); ++second_other_point)
		if(
			fruitcut::math::line::distance_to_point(
				*second_other_point,
				fruitcut::math::line::basic<scalar,3>(
					(*points.begin()),
					(*first_other_point) - (*points.begin()))) > epsilon)
			break;

	// All the points lie on one line
	if(second_other_point == points.end())
		return fcppt::optional<matrix4>();

	typedef
	fcppt::container::array<vector,2>
	direction_vectors;

	direction_vectors directions(
		fcppt::assign::make_array<vector>
			((*first_other_point) - (*points.begin()))
			((*second_other_point) - (*points.begin())));

	fcppt::math::vector::orthogonalize(
		directions.begin(),
		directions.end());

	for(
		typename direction_vectors::iterator i = directions.begin(); 
		i != directions.end(); 
		++i)
		(*i) = 
			fcppt::math::vector::normalize(
				*i);
		
	vector const crossed = 
		fcppt::math::vector::cross(
			directions[0],
			directions[1]);

	return 
		matrix4(
			directions[0][0],directions[1][0],crossed[0],(*points.begin())[0],
			directions[0][1],directions[1][1],crossed[1],(*points.begin())[1],
			directions[0][2],directions[1][2],crossed[2],(*points.begin())[2],
			0,0,0,1);
}

template<typename Output,typename Input>
Output const
vector_dimension_convert(
	Input const &input)
{
	Output result = Output::null();
	std::copy(
		input.begin(),
		input.begin() + result.size(),
		result.begin());
	return result;
}
}

fcppt::unique_ptr<fruitcut::app::fruit::cut_mesh_result>
fruitcut::app::fruit::cut_mesh(
	fruit::mesh const &input_mesh,
	fruit::plane const &input_plane)
{
	typedef
	sge::renderer::scalar
	scalar;

	typedef
	sge::renderer::vector2
	vector2;

	typedef
	sge::renderer::vector3
	vector3;

	typedef
	sge::renderer::vector4
	vector4;

	typedef
	sge::renderer::matrix4
	matrix4;

	typedef
	fcppt::math::box::basic<scalar,2>
	box2;

	typedef
	fcppt::math::box::basic<scalar,3>
	box3;

	fcppt::unique_ptr<fruit::cut_mesh_result> result = 
		fcppt::make_unique_ptr<fruit::cut_mesh_result>();

	scalar const epsilon = 
		static_cast<scalar>(
			0.01);

	typedef
	std::vector<vector3>
	vector3_sequence;

	vector3_sequence border;

	// First step: Collect all the triangles and the border points.
	for(
		mesh::triangle_sequence::const_iterator input_triangle = 
			input_mesh.triangles.begin(); 
		input_triangle != input_mesh.triangles.end(); 
		++input_triangle)
	{
		typedef
		math::triangle_plane_intersection<triangle> 
		intersection;

		intersection const single_result = 
			math::cut_triangle_at_plane(
				*input_triangle,
				input_plane);

		for(
			intersection::point_sequence::const_iterator current_is_point = 
				single_result.points().begin();
			current_is_point != single_result.points().end();
			++current_is_point)
			border.push_back(
				*current_is_point);
		// Only add new points if they're not already in the set
		/*
		BOOST_FOREACH(
			intersection::point_sequence::const_reference r,
			single_result.points())
			if(
				boost::range::find_if(
					border,
					boost::phoenix::bind(
						&fcppt::math::range_compare
						<
							sge::renderer::vector3,
							sge::renderer::vector3,
							sge::renderer::scalar
						>,
						boost::phoenix::arg_names::arg1,
						r,
						epsilon)) == border.end())
				border.push_back(
					r);
		*/

		// Copy all triangles
		boost::range::copy(
			single_result.triangles(),
			std::back_inserter(
				result->mesh().triangles));
	}

	// If there were no triangles, we can return
	if(result->mesh().triangles.empty())
		return 
			fcppt::move(
				result);

	// Step 2: Calculate the bounding box and the barycenter (we can do
	// that in one pass, luckily)
	result->barycenter() = vector3::null();

	vector3 
		min_pos = 
			result->mesh().triangles.front().vertices[0],
		max_pos = 
			min_pos;

	for(
		mesh::triangle_sequence::const_iterator current_tri = 
			result->mesh().triangles.begin(); 
		current_tri != result->mesh().triangles.end(); 
		++current_tri)
	{
		for(
			triangle::vertex_array::const_iterator current_vertex = 
				current_tri->vertices.begin();
			current_vertex != current_tri->vertices.end();
			++current_vertex)
		{
			result->barycenter() += *current_vertex;
			for (vector3::size_type i = 0; i < 3; ++i)
			{
				min_pos[i] = std::min(min_pos[i],(*current_vertex)[i]);
				max_pos[i] = std::max(max_pos[i],(*current_vertex)[i]);
			}
		}
	}

	result->bounding_box() = 
		box3(
			min_pos,
			fcppt::math::vector::structure_cast<box3::dim>(
				max_pos - min_pos));

	result->barycenter() /= 
		static_cast<scalar>(
			result->mesh().triangles.size() * 3);

	for(
		mesh::triangle_sequence::iterator current_tri = 
			result->mesh().triangles.begin(); 
		current_tri != result->mesh().triangles.end(); 
		++current_tri)
		for(
			triangle::vertex_array::iterator current_vertex = 
				current_tri->vertices.begin();
			current_vertex != current_tri->vertices.end();
			++current_vertex)
			(*current_vertex) -= result->barycenter();

	// If we've got no border, quit now. This happens when the cutting
	// plane doesn't intersect with the fruit at all (we "miss", so to
	// speak)
	if(border.size() < static_cast<vector3_sequence::size_type>(3))
		return 
			fcppt::move(
				result);

#if 0
	//std::cout << "border not empty!\n";
	std::cout << "------------------- Input points begin \n";
	std::cout << "{\n";
	BOOST_FOREACH(
		vector3_sequence::const_reference r,
		border)
		std::cout << "{" << r[0] << "," << r[1] << "," << r[2] << "},\n";
	std::cout << "}\n";
	std::cout << "------------------- Input points end\n";
#endif

	fcppt::optional<matrix4> const cs = 
		make_coordinate_system(
			border,
			epsilon);

	if(!cs)
		return
			fcppt::move(
				result);

	matrix4 const tcs = 
		*cs;

	matrix4 const coordinate_transformation = 
		fcppt::math::matrix::inverse(
			*cs);

	typedef
	boost::geometry::model::multi_point<vector2>
	point_cloud_2d;

	typedef
	boost::geometry::model::ring<vector2>
	ring_2d;

	point_cloud_2d reduced;
	reduced.reserve(
		border.size());

	for(vector3_sequence::const_iterator i = border.begin(); i != border.end(); ++i)
	{
		vector4 const transformed = 
			coordinate_transformation * 
			vector4(
				(*i)[0],
				(*i)[1],
				(*i)[2],
				// WATCH OUT: ONLY WORKS WITH THE 1 HERE!
				1);

		reduced.push_back(
			vector_dimension_convert<vector2>(
				transformed));

		FCPPT_ASSERT(
			transformed[2] < epsilon);

		// Warning: Costly assert!
		if(
			fcppt::math::vector::length(
				vector_dimension_convert<vector3>(
					(*cs) * vector4(reduced.back()[0],reduced.back()[1],0,1)) - 
				(*i)) > epsilon)
		{
			std::cout << 
				fcppt::math::vector::length(
					vector_dimension_convert<vector3>(
						(*cs) * vector4(reduced.back()[0],reduced.back()[1],0,1)) - 
					(*i)) << "\n";
			FCPPT_ASSERT(false);
		}
	}

	ring_2d convex_hull_result;

	boost::geometry::convex_hull(
		reduced,
		convex_hull_result);

	if(convex_hull_result.size() < static_cast<ring_2d::size_type>(3))
	{
		std::cerr << "Didn't get enough points, returning\n";
		return
			fcppt::move(
				result);
	}

	box2 const envelope = 
		boost::geometry::return_envelope<box2>(
			convex_hull_result);

	typedef
	std::vector<vector2>
	texcoord_vector;

	texcoord_vector texcoords;
	texcoords.reserve(
		convex_hull_result.size());

	for(ring_2d::const_iterator i = convex_hull_result.begin(); i != convex_hull_result.end(); ++i)
		texcoords.push_back(
			((*i) - envelope.pos())/
			vector2(
				envelope.w(),
				envelope.h()));

	ring_2d::size_type const triangle_count = 
		static_cast<ring_2d::size_type>(
			convex_hull_result.size() - 2u);

	result->mesh().triangles.reserve(
		result->mesh().triangles.size() + triangle_count);

	for(
		ring_2d::size_type current_vertex = 
			static_cast<ring_2d::size_type>(
				2);
		current_vertex < convex_hull_result.size();
		++current_vertex)
	{
		result->mesh().triangles.push_back(
			fruit::triangle(
				fcppt::assign::make_array<triangle::vector>
					(vector_dimension_convert<vector3>(
						(*cs) * 
						vector4(
							convex_hull_result[0][0],
							convex_hull_result[0][1],
							0,
							1)) - result->barycenter())
					(vector_dimension_convert<vector3>(
						(*cs) * 
						vector4(
							convex_hull_result[current_vertex-1][0],
							convex_hull_result[current_vertex-1][1],
							0,
							1)) - result->barycenter())
					(vector_dimension_convert<vector3>(
						(*cs) * 
						vector4(
							convex_hull_result[current_vertex][0],
							convex_hull_result[current_vertex][1],
							0,
							1)) - result->barycenter()),
				fcppt::assign::make_array<triangle::texcoord_type>
					(transform_texcoord(
						texcoords[
							0]))
					(transform_texcoord(
						texcoords[
							current_vertex-1]))
					(transform_texcoord(
						texcoords[
							current_vertex]))));

		result->area() += 
			triangle_area(
				result->mesh().triangles.back());
	}

	return 
		fcppt::move(
			result);
}
