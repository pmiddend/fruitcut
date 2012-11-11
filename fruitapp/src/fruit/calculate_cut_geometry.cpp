#include <fruitapp/renderer_dim2.hpp>
#include <fruitapp/renderer_rect.hpp>
#include <fruitapp/fruit/calculate_cut_geometry.hpp>
#include <fruitapp/fruit/cut_geometry.hpp>
#include <fruitapp/fruit/object.hpp>
#include <fruitlib/math/transform_direction.hpp>
#include <fruitlib/math/unproject.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/renderer/target/base.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/matrix/inverse.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/dot.hpp>


fruitapp::fruit::cut_geometry const
fruitapp::fruit::calculate_cut_geometry(
	fruitapp::fruit::hull::intersection_pair const &_intersection,
	sge::renderer::target::base const &_target,
	sge::camera::base const &_camera,
	fruitapp::fruit::object const &_current_fruit)
{
	sge::renderer::matrix4 const inverse_mvp =
		fcppt::math::matrix::inverse(
			sge::camera::matrix_conversion::world_projection(
				_camera.coordinate_system(),
				_camera.projection_matrix()));

	fruitapp::renderer_rect const viewport_rect(
		fruitapp::renderer_rect(
			sge::renderer::vector2::null(),
			fcppt::math::dim::structure_cast<fruitapp::renderer_dim2>(
				_target.viewport().get().size())));

	sge::renderer::matrix4 const transposed_rotation(
		fcppt::math::matrix::transpose(
			_current_fruit.rotation()));

	sge::renderer::vector3 const
		// Convert the points to 3D and to renderer::scalar
		point1(
			fcppt::math::vector::construct(
				_intersection.first,
				static_cast<sge::renderer::scalar>(
					0))),
		point2(
			fcppt::math::vector::construct(
				_intersection.second,
				static_cast<sge::renderer::scalar>(
					0))),
		point3(
			fcppt::math::vector::construct(
				_intersection.first,
				static_cast<sge::renderer::scalar>(
					0.5))),
		// unproject 'em
		point1_unprojected(
			fruitlib::math::unproject(
				point1,
				inverse_mvp,
				// The points are already "un-viewported", but they are in
				// screenspace, so use the screen rect here
				viewport_rect)),
		point2_unprojected(
			fruitlib::math::unproject(
				point2,
				inverse_mvp,
				// The points are already "un-viewported", but they are in
				// screenspace, so use the screen rect here
				viewport_rect)),
		point3_unprojected(
			fruitlib::math::unproject(
				point3,
				inverse_mvp,
				viewport_rect)),
		first_plane_vector(
			point2_unprojected - point1_unprojected),
		second_plane_vector(
			point3_unprojected - point1_unprojected),
		// NOTE: For rotation matrices M and vectors a,b the following holds:
		// cross(M*a,M*b) = M*cross(a,b)
		plane_normal(
			fruitlib::math::transform_direction(
				transposed_rotation,
				fcppt::math::vector::cross(
					first_plane_vector,
					second_plane_vector)));

	sge::renderer::scalar const plane_scalar =
		fcppt::math::vector::dot(
			fruitlib::math::transform_direction(
				transposed_rotation,
				point1_unprojected - _current_fruit.position()),
			plane_normal);

	return
		fruitapp::fruit::cut_geometry(
			fruitapp::fruit::cut_plane(
				fruitapp::fruit::plane(
					plane_normal,
					plane_scalar)),
			fruitapp::fruit::cut_direction(
				sge::renderer::vector3(
					first_plane_vector)));
}
