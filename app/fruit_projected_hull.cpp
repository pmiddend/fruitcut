#include "fruit_projected_hull.hpp"
#include "fruit.hpp"
#include "box3.hpp"
#include "../geometry_traits/vector.hpp"
#include "../geometry_traits/box.hpp"
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/target_base_ptr.hpp>
#include <sge/renderer/target_base.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vector4.hpp>
#include <sge/renderer/viewport.hpp>
#include <boost/geometry/geometries/adapted/std_as_ring.hpp>
#include <boost/geometry/geometry.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/math/box/corner_points.hpp>
#include <fcppt/math/vector/narrow_cast.hpp>
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <fcppt/math/matrix/output.hpp>
#include <fcppt/math/box/output.hpp>
#include <fcppt/math/dim/output.hpp>
#include <fcppt/math/vector/output.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <vector>

fruitcut::app::hull_point_sequence const
fruitcut::app::fruit_projected_hull(
	fruit const &f,
	sge::renderer::target_base_ptr const target,
	sge::renderer::matrix4 const &mvp)
{
	hull_point_sequence result;

	BOOST_FOREACH(
		box3::vector const &v,
		fcppt::math::box::corner_points(
			f.bounding_box()))
	{
		sge::renderer::vector4 const projected = 
			mvp 
				 * 
					f.world_transform() 
				* 
					fcppt::math::vector::construct(
						v,
						static_cast<box3::vector::value_type>(
							1));

		sge::renderer::vector2 const divided = 
			fcppt::math::vector::narrow_cast<hull_point_sequence::value_type>(
				projected / projected[3]);

		sge::renderer::vector2 const window_coordinates = 
			(divided + 
				sge::renderer::vector2(
					static_cast<sge::renderer::scalar>(1),
					static_cast<sge::renderer::scalar>(1)))
				* (fcppt::math::dim::structure_cast<sge::renderer::vector2>(
					// Fucking strong typedef shit
					target->viewport().get().dimension()) / static_cast<sge::renderer::scalar>(2)) 
				+ fcppt::math::vector::structure_cast<sge::renderer::vector2>(
						// Fucking strong typedef shit
						target->viewport().get().pos());

		result.push_back(
			window_coordinates);
	}

	return result;
}
