#include "cut_mesh.hpp"
#include "math/cut_triangle_at_plane.hpp"
#include "math/triangle_plane_intersection.hpp"
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
#include <boost/mpl/integral_c.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/bind.hpp>
#include <algorithm>
#include <vector>
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
}

fruitcut::mesh const
fruitcut::cut_mesh(
	mesh const &m,
	plane const &p)
{
	mesh result;

	// I'd rather make this a set, sorted by angle to the first point,
	// but what's the first point? ;)
	typedef
	std::vector<sge::renderer::vector3>
	point_sequence;

	point_sequence border;

	BOOST_FOREACH(
		mesh::triangle_sequence::const_reference t,
		m.triangles)
	{
		math::triangle_plane_intersection<triangle> const single_result = 
			fruitcut::math::cut_triangle_at_plane(
				t,
				p,
				&get_position,
				&get_data,
				&interpolate,
				&create_triangle);

		boost::range::copy(
			single_result.points(),
			std::back_inserter(
				border));

		boost::range::copy(
			single_result.triangles(),
			std::back_inserter(
				result.triangles));
	}

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

	typedef
	std::vector<point_sequence::size_type>
	index_sequence;

	index_sequence indices(
		border.size());

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
}
