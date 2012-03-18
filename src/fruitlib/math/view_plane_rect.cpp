#include <fruitlib/math/view_plane_distance.hpp>
#include <fruitlib/math/view_plane_rect.hpp>
#include <cmath>

fcppt::math::box::object<sge::renderer::scalar,2> const
fruitlib::math::view_plane_rect(
	sge::renderer::matrix4 const &mvp,
	sge::camera::projection::perspective const &perspective)
{
	typedef
	fcppt::math::box::object<sge::renderer::scalar,2>
	box_type;

	sge::renderer::scalar const
		distance_to_origin =
			math::view_plane_distance(
				mvp,
				perspective),
		height =
			static_cast<sge::renderer::scalar>(
				2 *
				std::tan(
					perspective.fov().get()/2) * distance_to_origin),
		width =
			static_cast<sge::renderer::scalar>(
				height * perspective.aspect().get());

	return
		box_type(
			box_type::vector(
				-width/2,
				-height/2),
			box_type::dim(
				width,
				height));
}
