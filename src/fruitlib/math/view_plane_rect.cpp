#include <fruitlib/math/view_plane_distance.hpp>
#include <fruitlib/math/view_plane_rect.hpp>
#include <fcppt/config/external_begin.hpp>
#include <cmath>
#include <fcppt/config/external_end.hpp>


fcppt::math::box::object<sge::renderer::scalar,2> const
fruitlib::math::view_plane_rect(
	sge::renderer::matrix4 const &mvp,
	fruitlib::perspective_projection_information const &_perspective)
{
	typedef
	fcppt::math::box::object<sge::renderer::scalar,2>
	box_type;

	sge::renderer::scalar const
		distance_to_origin =
			math::view_plane_distance(
				mvp,
				_perspective),
		height =
			static_cast<sge::renderer::scalar>(
				2 *
				std::tan(
					_perspective.fov().get()/2) * distance_to_origin),
		width =
			static_cast<sge::renderer::scalar>(
				height * _perspective.aspect().get());

	return
		box_type(
			box_type::vector(
				-width/2,
				-height/2),
			box_type::dim(
				width,
				height));
}
