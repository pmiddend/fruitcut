#include "cut_mesh.hpp"
#include "math/cut_triangle_at_plane.hpp"
#include "math/triangle_plane_intersection.hpp"
#include "math/generate_texture_coordinates.hpp"
#include "triangle.hpp"
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/angle_between.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/math/vector/dot.hpp>
#include <fcppt/math/detail/static_storage.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/assign/make_container.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/next_prior.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/operator/arithmetic.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/spirit/home/phoenix/operator/if_else.hpp>
#include <boost/spirit/home/phoenix/operator/comparison.hpp>
#include <boost/range/algorithm_ext/iota.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/min_element.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/bind.hpp>
#include <algorithm>
#include <vector>
#include <set>
#include <iterator>
#include <iostream>
#include <cmath>

namespace
{
template<typename T>
bool
nearly_equals(
	T const &a,
	T const &b,
	T const &epsilon)
{
	return std::abs(a-b) < epsilon;
}

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

sge::renderer::scalar
compare_angles(
	sge::renderer::vector2 const &p0,
	sge::renderer::vector2 const &p1,
	sge::renderer::vector2 const &p2)
{
	return (p1.x() - p0.x()) * (p2.y() - p0.y()) - (p2.x() - p0.x())*(p1.y() - p0.y());
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
		{
			if(
				std::find_if(
					border.begin(),
					border.end(),
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
		}

		boost::range::copy(
			single_result.triangles(),
			std::back_inserter(
				result.triangles));
	}

	typedef
	std::vector<sge::renderer::vector2>
	texcoord_vector;

	std::cout << "input points before: \n";
	BOOST_FOREACH(point_sequence::const_reference r,border)
		std::cout << "{" <<  r[0] << "," << r[1] << "," << r[2] << "}," << "\n";

	// No const here, we want to delete some of the points
	texcoord_vector texcoords = 
		math::generate_texture_coordinates<texcoord_vector>(
			border);

	typedef
	std::vector<texcoord_vector::size_type>
	index_sequence;

	index_sequence indices(
		border.size());

	boost::iota(
		indices,
		static_cast<index_sequence::value_type>(0));

	std::cout << "texcoords before:\n";
	BOOST_FOREACH(texcoord_vector::const_reference r,texcoords)
		std::cout << "{" << r[0] << "," << r[1] << "}," << "\n";

	// Step 1: Find the point with the lowest y coordinate
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

	std::swap(
		*min_y_it,
		*texcoords.begin());

	std::cout << "texcoords after swapping:\n";
	BOOST_FOREACH(texcoord_vector::const_reference r,texcoords)
		std::cout << "{" << r[0] << "," << r[1] << "}," << "\n";

	sge::renderer::vector2 const min_y = 
		*texcoords.begin();

	std::cout << "minimum texcoord:\n";
	std::cout << "{" << min_y[0] << "," << min_y[1] << "}," << "\n";

	typedef
	std::set<index_sequence::value_type>
	index_set;

	index_set to_delete;
	for (index_sequence::value_type i = 1; i < border.size(); ++i)
	{
		for (index_sequence::value_type j = i+1; j < border.size(); ++j)
		{
			if(
				std::abs(
					compare_angles(
						min_y,
						texcoords[i],
						texcoords[j])) < static_cast<sge::renderer::scalar>(0.0001))
			{
				to_delete.insert(
					fcppt::math::vector::length(min_y - texcoords[i]) 
						< fcppt::math::vector::length(min_y - texcoords[j]) 
					? 
						i
					:
						j);

				std::cout << "Deleting " << *(--to_delete.end()) << "\n";
			}
		}
	}

	BOOST_FOREACH(index_set::const_reference r,to_delete)
	{
		texcoords.erase(
			texcoords.begin() + r);
		border.erase(
			border.begin() + r);
	}

	std::cout << "texcoords after:\n";
	BOOST_FOREACH(texcoord_vector::const_reference r,texcoords)
		std::cout << "{" << r[0] << "," << r[1] << "}," << "\n";

	std::cout << "input points after: \n";
	BOOST_FOREACH(point_sequence::const_reference r,border)
		std::cout << "{" <<  r[0] << "," << r[1] << "," << r[2] << "}," << "\n";

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
	{
		std::cout 
			<< "Adding triangle: " 
			<< indices.front()
			<< ","
			<< (*i)
			<< ","
			<< (*boost::next(i))
			<< "\n";
		triangle::vertex_array const vertices = 
			{{
				border[indices.front()],
				border[*i],
				border[
					*boost::next(
						i)]
			}};
		result.triangles.push_back(
			triangle(
				vertices,
				triangle::texcoord_array()));
	}
	
	return result;
#if 0
	// Step 1: Fix the first point texcoords[0] and calculate the angle
	// between this point and all other points relative to the x axis
	// (so (1,0)).
	typedef
	std::vector<sge::renderer::scalar>
	scalar_sequence;

	scalar_sequence const angles = 
		fcppt::algorithm::map<scalar_sequence>(
			border,
			boost::phoenix::bind(
				&fcppt::math::vector::angle_between<>,
				boost::phoenix::arg_names::arg1 - border.front(),
				sge::renderer::vector2(1,0)));

	// iota, wohoooo \o/
	boost::range::iota(
		indices,
		static_cast<index_sequence::value_type>(0));

	boost::range::sort(
		indices,
		arg1);

	return result;
	// Now tesselate the cross section.

	// Step 1: Generate direction vectors (arrows) that originate from
	// the first point, border[0].
	// 
	// Warning: The first arrow is ill-defined!
	point_sequence const arrows = 
		fcppt::algorithm::map<point_sequence>(
			border,
			boost::phoenix::bind(
				&fcppt::math::vector::normalize
				<
					sge::renderer::scalar,
					boost::mpl::integral_c<fcppt::math::size_type,3>,
					fcppt::math::detail::static_storage<sge::renderer::scalar,3>::type
				>,
				boost::phoenix::arg_names::arg1 - border[0]));

	// Step 2: Generate the cross section plane normal (normal1); then
	// vector which is orthogonal to the plane normal and the first
	// arrow (which will be the "reference" arrow)
	sge::renderer::vector3 const 
		normal1 = 
			fcppt::math::vector::cross(
				arrows[1],
				arrows[2]),
		normal2 = 
			fcppt::math::vector::cross(
				normal1,
				arrows[1]);

	typedef
	std::vector<sge::renderer::scalar>
	scalar_sequence;

	// Step 3: Calculate the _signed_ angle between the reference arrow
	// (the first arrow) and the other arrows. We can then sort the
	// arrows according to this angles and create the triangles from
	// this
	//
	// To calculate the signed angle we check if the dot between the
	// normal2 (see above) and the vector is greater than zero. If so,
	// the arrow is on the "other side" of the plane with normal2.
	scalar_sequence angles = 
		fcppt::algorithm::map<scalar_sequence>(
			arrows,
			boost::phoenix::if_else(
				boost::phoenix::bind(
					&fcppt::math::vector::dot
					<
						sge::renderer::scalar,
						boost::mpl::integral_c<fcppt::math::size_type,3>,
						fcppt::math::detail::static_storage<sge::renderer::scalar,3>::type,
						fcppt::math::detail::static_storage<sge::renderer::scalar,3>::type
					>,
					normal2,
					boost::phoenix::arg_names::arg1) > static_cast<sge::renderer::scalar>(0),
				boost::phoenix::val(
					static_cast<sge::renderer::scalar>(-1)),
				boost::phoenix::val(
					static_cast<sge::renderer::scalar>(1))) * 
			boost::phoenix::bind(
				&fcppt::math::vector::angle_between
				<
					sge::renderer::scalar,
					boost::mpl::integral_c<fcppt::math::size_type,3>,
					fcppt::math::detail::static_storage<sge::renderer::scalar,3>::type,
					fcppt::math::detail::static_storage<sge::renderer::scalar,3>::type
				>,
				boost::phoenix::val(
					arrows[1]),
				boost::phoenix::arg_names::arg1));

	// Step 4: Sort the arrows according to the angles. Since we cannot
	// sort the elements of container "a" with respect to the values in
	// vector "b" directly (it's just not possible) we sort the
	// _indices_ [0,...,n] according to the angles and then access the
	// point vector accordingly
	typedef
	std::vector<point_sequence::size_type>
	index_sequence;

	index_sequence indices(
		border.size());

	// iota, wohoooo \o/
	boost::range::iota(
		indices,
		static_cast<index_sequence::value_type>(0));

	boost::range::sort(
		indices,
		boost::phoenix::ref(angles)[boost::phoenix::arg_names::arg1] 
			< boost::phoenix::ref(angles)[boost::phoenix::arg_names::arg2]);

	for(
		index_sequence::const_iterator i = boost::next(indices.begin()); 
		i != boost::prior(indices.end()); 
		++i)
	{
		triangle::vertex_array const vertices = 
			{{
				border[indices.front()],
				border[*i],
				border[
					*boost::next(
						i)]
			}};
		result.triangles.push_back(
			triangle(
				vertices,
				triangle::texcoord_array()));
	}

	return result;
#endif
}
