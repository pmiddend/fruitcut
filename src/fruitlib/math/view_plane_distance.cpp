#include <fruitlib/math/view_plane_distance.hpp>
#include <fruitlib/math/plane/basic.hpp>
#include <fruitlib/math/plane/normalize.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/static.hpp>


sge::renderer::scalar
fruitlib::math::view_plane_distance(
	sge::renderer::matrix4 const &m,
	sge::camera::projection::perspective const &p)
{
	typedef
	math::plane::basic<sge::renderer::scalar,3>
	plane_type;

	typedef
	fcppt::math::vector::static_<sge::renderer::scalar,4>::type
	vector4;

	typedef
	fcppt::math::vector::static_<sge::renderer::scalar,3>::type
	vector3;

	vector4 const
		fourth_row(
			m[3][0],
			m[3][1],
			m[3][2],
			m[3][3]),
		third_row(
			m[2][0],
			m[2][1],
			m[2][2],
			m[2][3]),
		plane_vec4 =
			fourth_row + third_row;

	plane_type const near_plane =
		math::plane::normalize(
			plane_type(
				vector3(
					plane_vec4[0],
					plane_vec4[1],
					plane_vec4[2]),
				plane_vec4[3]));

	return
		near_plane.lambda() + p.near().get();
}
