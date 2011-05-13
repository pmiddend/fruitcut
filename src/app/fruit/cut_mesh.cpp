#include "cut_mesh.hpp"
#include "../../math/cut_triangle_at_plane.hpp"
#include "../../math/triangle_plane_intersection.hpp"
#include "../../math/generate_texture_coordinates.hpp"
#include "triangle.hpp"
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vector4.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/assign/make_array.hpp>
#include <boost/foreach.hpp>
#include <boost/next_prior.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/operator/arithmetic.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/spirit/home/phoenix/operator/if_else.hpp>
#include <boost/spirit/home/phoenix/operator/comparison.hpp>
#include <boost/range/algorithm_ext/iota.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/min_element.hpp>
#include <boost/range/algorithm/copy.hpp>

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <boost/geometry/multi/algorithms/detail/for_each_range.hpp>
#include "../../geometry_traits/vector.hpp"
#include "../../geometry_traits/box.hpp"

#include <algorithm>
#include <vector>
#include <set>
#include <iterator>
#include <iostream>
#include <cmath>

namespace
{
fruitcut::app::fruit::triangle const
create_triangle(
	sge::renderer::vector3 const &p0,
	sge::renderer::vector2 const &t0,
	sge::renderer::vector3 const &p1,
	sge::renderer::vector2 const &t1,
	sge::renderer::vector3 const &p2,
	sge::renderer::vector2 const &t2)
{
	fruitcut::app::fruit::triangle::vertex_array const vertices = {{ p0,p1,p2 }};
	fruitcut::app::fruit::triangle::texcoord_array const texcoords = {{ t0,t1,t2 }};
	return 
		fruitcut::app::fruit::triangle(
			vertices,
			texcoords);
}

sge::renderer::vector3 const
get_position(
	fruitcut::app::fruit::triangle const &t,
	fruitcut::app::fruit::triangle::size_type const i)
{
	return t.vertices[i];
}

sge::renderer::vector2 const
get_data(
	fruitcut::app::fruit::triangle const &t,
	fruitcut::app::fruit::triangle::size_type const i)
{
	return t.texcoords[i];
}

sge::renderer::vector2 const
interpolate(
	sge::renderer::vector2 const &a,
	sge::renderer::vector2 const &b,
	sge::renderer::scalar c)
{
	return a * c + (static_cast<sge::renderer::scalar>(1)-c) * b;
}

// Two helper functions for the cross section
template<typename T>
bool
nearly_equals(
	T const &a,
	T const &b,
	T const &epsilon)
{
	return std::abs(a-b) < epsilon;
}

sge::renderer::scalar
compare_angles(
	sge::renderer::vector2 const &p0,
	sge::renderer::vector2 const &p1,
	sge::renderer::vector2 const &p2)
{
	return (p1.x() - p0.x()) * (p2.y() - p0.y()) - (p2.x() - p0.x())*(p1.y() - p0.y());
}

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
struct make_coordinate_system_msvc_hack
{
	typedef typename
	Container::value_type::value_type
	scalar_type

	typedef typename
	fcppt::math::matrix::static_<scalar_type,4,4>::type
	type;
};

template<typename Container>
fcppt::optional
<
	typename
	make_coordinate_system_msvc_hack<Container>::type
> const
make_coordinate_system(
	Container const &points,
	typename Container::value_type::value_type const epsilon)
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

void
fruitcut::app::fruit::cut_mesh(
	mesh const &input_mesh,
	plane const &input_plane,
	mesh &output_mesh,
	box3 &output_bounding_box,
	fruit::area &output_area,
	sge::renderer::vector3 &output_barycenter)
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
				input_plane,
				&get_position,
				&get_data,
				&interpolate,
				&create_triangle);

		for(
			intersection::point_sequence::const_iterator current_is_point = 
				single_result.points().begin();
			current_is_point != single_result.points().end();
			++current_is_point)
			border.push_back(
				*current_is_point);
		/*
		// Only add new points if they're not already in the set
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
				output_mesh.triangles));
	}

	// If there were no triangles, we can return
	if(output_mesh.triangles.empty())
		return;

	// Step 2: Calculate the bounding box and the barycenter (we can do
	// that in one pass, luckily)
	output_barycenter = vector3::null();

	vector3 
		min_pos = 
			output_mesh.triangles.front().vertices[0],
		max_pos = 
			min_pos;

	for(
		mesh::triangle_sequence::const_iterator current_tri = 
			output_mesh.triangles.begin(); 
		current_tri != output_mesh.triangles.end(); 
		++current_tri)
	{
		for(
			triangle::vertex_array::const_iterator current_vertex = 
				current_tri->vertices.begin();
			current_vertex != current_tri->vertices.end();
			++current_vertex)
		{
			output_barycenter += *current_vertex;
			for (vector3::size_type i = 0; i < 3; ++i)
			{
				min_pos[i] = std::min(min_pos[i],(*current_vertex)[i]);
				max_pos[i] = std::max(max_pos[i],(*current_vertex)[i]);
			}
		}
	}

	output_bounding_box = 
		box3(
			min_pos,
			fcppt::math::vector::structure_cast<box3::dim>(
				max_pos - min_pos));

	output_barycenter /= 
		static_cast<scalar>(
			output_mesh.triangles.size() * 3);

	for(
		mesh::triangle_sequence::iterator current_tri = 
			output_mesh.triangles.begin(); 
		current_tri != output_mesh.triangles.end(); 
		++current_tri)
		for(
			triangle::vertex_array::iterator current_vertex = 
				current_tri->vertices.begin();
			current_vertex != current_tri->vertices.end();
			++current_vertex)
			(*current_vertex) -= output_barycenter;

	// If we've got no border, quit now. This happens when the cutting
	// plane doesn't intersect with the fruit at all (we "miss", so to
	// speak)
	if(border.size() < static_cast<vector3_sequence::size_type>(3))
		return;

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
		return;

	matrix4 const tcs = 
		*cs;

#if 0
	std::cout 
		<< "coordinate system:\n" 
		<< "{"
		<< "{" << tcs[0][0] << ","<< tcs[0][1] << ","<< tcs[0][2] << ","<< tcs[0][3] << "},\n"
		<< "{" << tcs[1][0] << ","<< tcs[1][1] << ","<< tcs[1][2] << ","<< tcs[1][3] << "},\n"
		<< "{" << tcs[2][0] << ","<< tcs[2][1] << ","<< tcs[2][2] << ","<< tcs[2][3] << "},\n"
		<< "{" << tcs[3][0] << ","<< tcs[3][1] << ","<< tcs[3][2] << ","<< tcs[3][3] << "}\n"
		<< "}\n";
#endif

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

#if 0
	std::cout << "------------------ INPUT ------------------\n";
	std::cout << "{\n";
#endif

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

#if 0
		std::cout << "{" << transformed[0] << "," << transformed[1] << "}";
		if (i != (--border.end()))
			std::cout << ",";
		std::cout << "\n";
#endif

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

#if 0
	std::cout << "}\n";
	std::cout << "------------------ INPUT ------------------\n";
#endif

	ring_2d result;

	boost::geometry::convex_hull(
		reduced,
		result);

	if(result.size() < static_cast<ring_2d::size_type>(3))
	{
		std::cerr << "Didn't get enough points, returning\n";
		return;
	}

	/*
	std::cout << "Reduced " << (reduced.size() - result.size()) << " points\n";
	std::cout << "------------------ OUTPUT ------------------\n";
	std::cout << "{\n";

	for(ring_2d::const_iterator i = result.begin(); i != result.end(); ++i)
	{
		std::cout << "{" << (*i)[0] << "," << (*i)[1] << "}";
		if (i != (--result.end()))
			std::cout << ",";
		std::cout << "\n";
	}

	std::cout << "}\n";
	std::cout << "------------------ OUTPUT ------------------\n";
	*/

	box2 const envelope = 
		boost::geometry::return_envelope<box2>(
			result);

	typedef
	std::vector<vector2>
	texcoord_vector;

	texcoord_vector texcoords;
	texcoords.reserve(
		result.size());

	for(ring_2d::const_iterator i = result.begin(); i != result.end(); ++i)
		texcoords.push_back(
			((*i) - envelope.pos())/
			vector2(
				envelope.w(),
				envelope.h()));

	ring_2d::size_type const triangle_count = 
		static_cast<ring_2d::size_type>(
			result.size() - 2u);

	output_mesh.triangles.reserve(
		output_mesh.triangles.size() + triangle_count);

	for(
		ring_2d::size_type current_vertex = 
			static_cast<ring_2d::size_type>(
				2);
		current_vertex < result.size();
		++current_vertex)
	{
		output_mesh.triangles.push_back(
			fruit::triangle(
				fcppt::assign::make_array<triangle::vector>
					(vector_dimension_convert<vector3>(
						(*cs) * 
						vector4(
							result[0][0],
							result[0][1],
							0,
							1)) - output_barycenter)
					(vector_dimension_convert<vector3>(
						(*cs) * 
						vector4(
							result[current_vertex-1][0],
							result[current_vertex-1][1],
							0,
							1)) - output_barycenter)
					(vector_dimension_convert<vector3>(
						(*cs) * 
						vector4(
							result[current_vertex][0],
							result[current_vertex][1],
							0,
							1)) - output_barycenter),
				fcppt::assign::make_array<triangle::data_type>
					(transform_texcoord(
						texcoords[
							0]))
					(transform_texcoord(
						texcoords[
							current_vertex-1]))
					(transform_texcoord(
						texcoords[
							current_vertex-2]))));

		output_area += 
			triangle_area(
				output_mesh.triangles.back());
	}


	







#if 0





	typedef
	std::vector<sge::renderer::vector2>
	texcoord_vector;

	// No const here, we want to delete some of the points
	texcoord_vector texcoords = 
		border.empty()
		?
			texcoord_vector()
		:
			math::generate_texture_coordinates<texcoord_vector>(
				border);

	/*
	std::cout << "------------------- Texcoords begin \n";
	BOOST_FOREACH(
		texcoord_vector::const_reference r,
		texcoords)
		std::cout << "{" << r[0] << "," << r[1] << "},\n";
	std::cout << "------------------- Texcoords end \n";
	*/

	typedef
	std::vector<texcoord_vector::size_type>
	index_sequence;

	// We cannot sort a container 'A' with a criterion which is stored
	// in container 'B', so we have to detour over this index_sequence
	index_sequence indices(
		border.size());

	boost::iota(
		indices,
		static_cast<index_sequence::value_type>(0));

	// Step 1: Find the point with the lowest y coordinate (note the
	// sloppy equality and the lexicographical comparison)
	texcoord_vector::iterator min_y_it = 
		boost::min_element(
			texcoords,
			boost::phoenix::if_else(
				boost::phoenix::bind(
					&nearly_equals<sge::renderer::scalar>,
					boost::phoenix::arg_names::arg1[1],
					boost::phoenix::arg_names::arg2[1],
					epsilon),
				boost::phoenix::arg_names::arg1[0] < 
					boost::phoenix::arg_names::arg2[0],
				boost::phoenix::arg_names::arg1[1] < 
					boost::phoenix::arg_names::arg2[1]));

	//std::cout << "Minimalpunkt: " << "{" << (*min_y_it)[0] << "," << (*min_y_it)[1] << "},\n";

	// WLOG, the first element is the one with the lowest y coordinates
	std::swap(
		*min_y_it,
		*texcoords.begin());
	std::swap(
		*(border.begin() + std::distance(texcoords.begin(),min_y_it)),
		*border.begin());

	sge::renderer::vector2 const min_y = 
		*texcoords.begin();

	// This is a set because maybe we delete a point more than once
	typedef
	std::set
	<
		index_sequence::value_type,
		std::greater<index_sequence::value_type>
	>
	index_set;

	// I think this is technically an optimization: If "min_y" and two
	// other points are colinear, delete the nearest point after min_y.
	// TODO: Can we make this more pretty?
	index_set to_delete;
	for (index_sequence::value_type i = 1; i < border.size(); ++i)
		for (index_sequence::value_type j = i+1; j < border.size(); ++j)
			if(
				std::abs(
					compare_angles(
						min_y,
						texcoords[i],
						texcoords[j])) < epsilon)
				to_delete.insert(
					fcppt::math::vector::length(min_y - texcoords[i]) 
						< fcppt::math::vector::length(min_y - texcoords[j]) 
					? 
						i
					:
						j);

//	std::cout << "Deleted texcoords begin\n";
	BOOST_FOREACH(
		index_set::const_reference r,
		to_delete)
	{
	//	std::cout << "{" << (*(texcoords.begin() + static_cast<texcoord_vector::difference_type>(r)))[0] << "," << (*(texcoords.begin() + static_cast<texcoord_vector::difference_type>(r)))[1] << "},\n";
		texcoords.erase(
			texcoords.begin() + static_cast<texcoord_vector::difference_type>(r));
		border.erase(
			border.begin() + static_cast<texcoord_vector::difference_type>(r));
	}
//	std::cout << "Deleted texcoords end\n";

	// Now we have to create a new index array!
	indices.resize(
		border.size());
	boost::iota(
		indices,
		static_cast<index_sequence::value_type>(0));

	// Finally, sort the indices by the texcoord angles
	std::sort(
		boost::next(
			indices.begin()),
		indices.end(),
		boost::phoenix::bind(
			&compare_angles,
			min_y,
			boost::phoenix::ref(texcoords)[boost::phoenix::arg_names::arg1],
			boost::phoenix::ref(texcoords)[boost::phoenix::arg_names::arg2]) 
			< static_cast<sge::renderer::scalar>(0));

//	std::cout << "Sorted points begin\n";
	/*
	BOOST_FOREACH(
		index_sequence::const_reference r,
		indices)
	*/
	//	std::cout << "{" << border[r][0] << "," << border[r][1] << "," << border[r][2] << "},\n";
//	std::cout << "Sorted points end\n";

	for(
		index_sequence::const_iterator i = boost::next(indices.begin()); 
		i != boost::prior(indices.end()); 
		++i)
	{
		result_mesh.triangles.push_back(
			triangle(
				fcppt::assign::make_array<triangle::vector>
					(border[indices.front()] - barycenter)
					(border[*i] - barycenter)
					(border[*boost::next(i)] - barycenter),
				fcppt::assign::make_array<triangle::data_type>
					(transform_texcoord(
						texcoords[indices.front()]))
					(transform_texcoord(
						texcoords[*i]))
					(transform_texcoord(
						texcoords[
							*boost::next(
								i)]))));
			
		area += 
			triangle_area(
				result_mesh.triangles.back());
	}
#endif
}
