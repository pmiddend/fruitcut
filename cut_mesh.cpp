#include "cut_mesh.hpp"
#include "math/cut_triangle_at_plane.hpp"
#include "math/triangle_plane_intersection.hpp"
#include "math/generate_texture_coordinates.hpp"
#include "triangle.hpp"
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
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
#include <algorithm>
#include <vector>
#include <set>
#include <iterator>
#include <iostream>
#include <cmath>

namespace
{
fruitcut::triangle const
create_triangle(
	sge::renderer::vector3 const &p0,
	sge::renderer::vector2 const &t0,
	sge::renderer::vector3 const &p1,
	sge::renderer::vector2 const &t1,
	sge::renderer::vector3 const &p2,
	sge::renderer::vector2 const &t2)
{
	fruitcut::triangle::vertex_array const vertices = {{ p0,p1,p2 }};
	fruitcut::triangle::texcoord_array const texcoords = {{ t0,t1,t2 }};
	return 
		fruitcut::triangle(
			vertices,
			texcoords);
}

sge::renderer::vector3 const
get_position(
	fruitcut::triangle const &t,
	fruitcut::triangle::size_type const i)
{
	return t.vertices[i];
}

sge::renderer::vector2 const
get_data(
	fruitcut::triangle const &t,
	fruitcut::triangle::size_type const i)
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
}

fruitcut::mesh const
fruitcut::cut_mesh(
	mesh const &m,
	plane const &p)
{
	mesh result;

	sge::renderer::scalar const epsilon = 
		static_cast<sge::renderer::scalar>(
			0.0001);

	typedef
	std::vector<sge::renderer::vector3>
	point_sequence;

	point_sequence border;

	BOOST_FOREACH(
		mesh::triangle_sequence::const_reference t,
		m.triangles)
	{
		typedef
		math::triangle_plane_intersection<triangle> 
		intersection;

		intersection const single_result = 
			fruitcut::math::cut_triangle_at_plane(
				t,
				p,
				&get_position,
				&get_data,
				&interpolate,
				&create_triangle);

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

		// Copy all triangles
		boost::range::copy(
			single_result.triangles(),
			std::back_inserter(
				result.triangles));
	}

	typedef
	std::vector<sge::renderer::vector2>
	texcoord_vector;

	// No const here, we want to delete some of the points
	texcoord_vector texcoords = 
		math::generate_texture_coordinates<texcoord_vector>(
			border);

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

	// WLOG, the first element is the one with the lowest y coordinates
	std::swap(
		*min_y_it,
		*texcoords.begin());

	sge::renderer::vector2 const min_y = 
		*texcoords.begin();

	// This is a set because maybe we delete a point more than once
	typedef
	std::set<index_sequence::value_type>
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

	BOOST_FOREACH(
		index_set::const_reference r,
		to_delete)
	{
		texcoords.erase(
			texcoords.begin() + static_cast<texcoord_vector::difference_type>(r));
		border.erase(
			border.begin() + static_cast<texcoord_vector::difference_type>(r));
	}

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

	for(
		index_sequence::const_iterator i = boost::next(indices.begin()); 
		i != boost::prior(indices.end()); 
		++i)
		result.triangles.push_back(
			triangle(
				fcppt::assign::make_array<triangle::vector>
					(border[indices.front()])
					(border[*i])
					(border[*boost::next(i)]),
				fcppt::assign::make_array<triangle::data_type>
					(transform_texcoord(
						texcoords[indices.front()]))
					(transform_texcoord(
						texcoords[*i]))
					(transform_texcoord(
						texcoords[
							*boost::next(
								i)]))));
	
	return result;
}
