#include <fruitlib/math/view_plane_distance.hpp>
#include <fruitlib/math/plane/basic.hpp>
#include <fruitlib/math/plane/normalize.hpp>
#include <fcppt/math/matrix/at_c.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/static.hpp>


sge::renderer::scalar
fruitlib::math::view_plane_distance(
	sge::renderer::matrix4 const &m,
	fruitlib::perspective_projection_information const &p)
{
	typedef
	math::plane::basic<sge::renderer::scalar,3>
	plane_type;

	typedef
	fcppt::math::vector::static_<sge::renderer::scalar,4>
	vector4;

	typedef
	fcppt::math::vector::static_<sge::renderer::scalar,3>
	vector3;

	vector4 const
		fourth_row(
			fcppt::math::matrix::at_c<
				3
			>(
				m
			)
		),
		third_row(
			fcppt::math::matrix::at_c<
				2
			>(
				m
			)
		),
		plane_vec4 =
			fourth_row + third_row;

	plane_type const near_plane =
		math::plane::normalize(
			plane_type(
				vector3(
					plane_vec4.x(),
					plane_vec4.y(),
					plane_vec4.z()
				),
				plane_vec4.w()
			)
		);

	return
		near_plane.lambda() + p.near().get();
}
