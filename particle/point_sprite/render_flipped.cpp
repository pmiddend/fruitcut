#include "render_flipped.hpp"
#include <sge/sprite/intrusive/system_impl.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/sprite/render_states.hpp>
#include <sge/sprite/transform_matrix.hpp>
#include <sge/sprite/projection_matrix.hpp>
#include <sge/sprite/matrix.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/matrix_mode.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/scaling.hpp>

void
fruitcut::particle::point_sprite::render_flipped(
	system &ss)
{
	sge::renderer::state::scoped const state(
		ss.renderer(),
		sge::sprite::render_states());

	ss.renderer()->transform(
		sge::renderer::matrix_mode::world,
		sge::sprite::transform_matrix(
			ss.renderer()->screen_size()));

	ss.renderer()->transform(
		sge::renderer::matrix_mode::projection,
		sge::sprite::projection_matrix() * 
			fcppt::math::matrix::scaling(
				static_cast<sge::sprite::matrix::value_type>(
					1),
				static_cast<sge::sprite::matrix::value_type>(
					-1),
				static_cast<sge::sprite::matrix::value_type>(
					1)));

	ss.render_all_advanced(
		sge::sprite::default_equal());
}
