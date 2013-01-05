#include <fruitapp/fruit/box3.hpp>
#include <fruitapp/fruit/object.hpp>
#include <fruitapp/fruit/hull/projected.hpp>
#include <fruitlib/geometry_traits/box.hpp>
#include <fruitlib/geometry_traits/vector.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vector4.hpp>
#include <sge/renderer/target/base.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/math/box/corner_points.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/box/output.hpp>
#include <fcppt/math/dim/output.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/output.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/vector/narrow_cast.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <boost/geometry/multi/algorithms/detail/for_each_range.hpp>
#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <array>
#include <fcppt/config/external_end.hpp>


fruitapp::fruit::hull::ring const
fruitapp::fruit::hull::projected(
	fruitapp::fruit::object const &f,
	sge::renderer::target::base const &target,
	sge::renderer::matrix4 const &mvp)
{
	typedef
	boost::geometry::model::multi_point
	<
		fruitapp::fruit::hull::ring::value_type
	>
	hull_point_cloud;

	hull_point_cloud point_cloud;

	typedef
	std::array<fruitapp::fruit::box3::vector,8>
	corner_point_array;

	corner_point_array const corner_points =
		fcppt::math::box::corner_points(
			f.bounding_box());

	for(
		corner_point_array::const_iterator v =
			corner_points.begin();
		v != corner_points.end();
		++v)
	{
		sge::renderer::vector4 const projected =
			mvp
				 *
					f.world_transform()
				*
					fcppt::math::vector::construct(
						(*v),
						static_cast<box3::vector::value_type>(
							1));

		sge::renderer::vector2 const divided =
			fcppt::math::vector::narrow_cast<hull_point_cloud::value_type>(
				projected / projected[3]);

		sge::renderer::vector2 const window_coordinates =
			(divided +
				sge::renderer::vector2(
					static_cast<sge::renderer::scalar>(1),
					static_cast<sge::renderer::scalar>(1)))
				* (fcppt::math::dim::structure_cast<sge::renderer::vector2>(
					// Fucking strong typedef shit
					target.viewport().get().size()) / static_cast<sge::renderer::scalar>(2));

		point_cloud.push_back(
			window_coordinates);
	}

	ring result;

	boost::geometry::convex_hull(
		point_cloud,
		result);

	return result;
}
