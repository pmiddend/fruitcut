#include <fruitapp/media_path.hpp>
#include <fruitapp/background/ffp.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/state/core/sampler/object.hpp>
#include <sge/renderer/state/core/sampler/parameters.hpp>
#include <sge/renderer/state/core/sampler/scoped.hpp>
#include <sge/renderer/state/core/sampler/address/mode_all.hpp>
#include <sge/renderer/state/core/sampler/filter/default.hpp>
#include <sge/renderer/state/ffp/transform/object.hpp>
#include <sge/renderer/state/ffp/transform/object_scoped_ptr.hpp>
#include <sge/renderer/state/ffp/transform/parameters.hpp>
#include <sge/renderer/state/ffp/transform/scoped.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/scoped.hpp>
#include <sge/shader/context.hpp>
#include <sge/shader/scoped_pair.hpp>
#include <fcppt/make_cref.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_map.hpp>


fruitapp::background::ffp::ffp(
	sge::renderer::device::ffp &_renderer,
	fruitlib::texture_manager &_texture_manager,
	fruitapp::background::repetitions const &_repetitions,
	sge::camera::base const &_camera,
	fruitapp::projection_manager::object &_projection_manager)
:
	fruitapp::background::base(
		_renderer,
		_texture_manager,
		_camera,
		_projection_manager,
		_repetitions),
	renderer_(
		_renderer),
	camera_(
		_camera),
	background_sampler_(
		_renderer.create_sampler_state(
			sge::renderer::state::core::sampler::parameters(
				sge::renderer::state::core::sampler::address::mode_all(
					sge::renderer::state::core::sampler::address::mode::repeat),
				sge::renderer::state::core::sampler::filter::default_()))),
	samplers_(
		fcppt::assign::make_map<sge::renderer::state::core::sampler::const_object_ref_map>
			(sge::renderer::texture::stage(
				0u),
			fcppt::make_cref(
				*background_sampler_)))
{
}

fruitapp::background::ffp::~ffp()
{
}

void
fruitapp::background::ffp::render(
	sge::renderer::context::core &_context)
{
	sge::renderer::state::core::sampler::scoped const scoped_sampler(
		_context,
		samplers_);

	sge::renderer::texture::scoped const scoped_texture(
		_context,
		fruitapp::background::base::texture(),
		sge::renderer::texture::stage(
			0u));

	sge::renderer::state::ffp::transform::object_scoped_ptr const projection_state(
		renderer_.create_transform_state(
			sge::renderer::state::ffp::transform::parameters(
				camera_.projection_matrix().get())));

	sge::renderer::state::ffp::transform::object_scoped_ptr const world_state(
		renderer_.create_transform_state(
			sge::renderer::state::ffp::transform::parameters(
				sge::camera::matrix_conversion::world(
					camera_.coordinate_system()))));

	sge::renderer::state::ffp::transform::scoped const projection_transform(
		dynamic_cast<sge::renderer::context::ffp &>(
			_context),
		sge::renderer::state::ffp::transform::mode::projection,
		*projection_state);

	sge::renderer::state::ffp::transform::scoped const world_transform(
		dynamic_cast<sge::renderer::context::ffp &>(
			_context),
		sge::renderer::state::ffp::transform::mode::world,
		*world_state);

	this->do_render(
		_context);
}
