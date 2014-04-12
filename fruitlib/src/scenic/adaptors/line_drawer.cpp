#include <fruitlib/scenic/adaptors/line_drawer.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/line_drawer/object.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/core.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/projection/far.hpp>
#include <sge/renderer/projection/near.hpp>
#include <sge/renderer/projection/orthogonal.hpp>
#include <sge/renderer/projection/rect.hpp>
#include <sge/renderer/state/ffp/transform/object.hpp>
#include <sge/renderer/state/ffp/transform/object_unique_ptr.hpp>
#include <sge/renderer/state/ffp/transform/parameters.hpp>
#include <sge/renderer/state/ffp/transform/scoped.hpp>
#include <sge/renderer/target/onscreen.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/matrix/object_impl.hpp>


namespace
{
sge::renderer::projection::rect const
projection_rect_from_viewport(
	sge::renderer::pixel_rect const &viewport)
{
	return
		sge::renderer::projection::rect(
			sge::renderer::projection::rect::vector(
				static_cast<sge::renderer::scalar>(
					viewport.left()),
				static_cast<sge::renderer::scalar>(
					viewport.bottom())),
			sge::renderer::projection::rect::dim(
				static_cast<sge::renderer::scalar>(
					viewport.size().w()),
				static_cast<sge::renderer::scalar>(
					-viewport.size().h())));
}
}

fruitlib::scenic::adaptors::line_drawer::line_drawer(
	fruitlib::scenic::optional_parent const &_parent,
	sge::line_drawer::object &_object,
	optional_renderer const &_renderer)
:
	node_base(
		_parent),
	object_(
		_object),
	renderer_(
		_renderer)
{
}

fruitlib::scenic::adaptors::line_drawer::~line_drawer()
{
}

void
fruitlib::scenic::adaptors::line_drawer::react(
	fruitlib::scenic::events::render const &_render_event)
{
	if(renderer_)
	{
		sge::renderer::state::ffp::transform::object_unique_ptr const world_state(
			renderer_->create_transform_state(
				sge::renderer::state::ffp::transform::parameters(
					sge::renderer::matrix4::identity())));

		sge::renderer::state::ffp::transform::scoped const world_transform(
			_render_event.context(),
			sge::renderer::state::ffp::transform::mode::world,
			*world_state);

		sge::renderer::state::ffp::transform::object_unique_ptr const projection_state(
			renderer_->create_transform_state(
				sge::renderer::state::ffp::transform::parameters(
					sge::renderer::projection::orthogonal(
						projection_rect_from_viewport(
							renderer_->onscreen_target().viewport().get()),
						sge::renderer::projection::near(
							0.f),
						sge::renderer::projection::far(
							10.f)))));

		sge::renderer::state::ffp::transform::scoped const projection_transform(
			_render_event.context(),
			sge::renderer::state::ffp::transform::mode::projection,
			*projection_state);

		object_.render(
			_render_event.context());
	}
	else
		object_.render(
			_render_event.context());
}
