#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/device.hpp>
#include <fruitapp/overlay.hpp>
#include <sge/renderer/clear/parameters.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <fruitapp/postprocessing.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/image/colors.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/viewport_size.hpp>
#include <fcppt/math/dim/object_impl.hpp>


fruitapp::overlay::overlay(
	fruitlib::scenic::optional_parent const &_parent,
	sge::renderer::device &_renderer,
	fruitapp::postprocessing &_postprocessing)
:
	node_base(
		_parent),
	renderer_(
		_renderer),
	postprocessing_(
		_postprocessing)
{
}

fruitapp::overlay::~overlay()
{
}

void
fruitapp::overlay::react(
	fruitlib::scenic::events::update const &e)
{
	// Update my children
	node_base::forward_to_children(
		e);

	// Do we even have a viewport?
	if (!sge::renderer::viewport_size(renderer_).content())
		return;

	renderer_.onscreen_target().clear(
		sge::renderer::clear::parameters()
			.back_buffer(
				sge::image::colors::black())
			.depth_buffer(
				sge::renderer::clear::depth_buffer_value(
					1.0f)));

	sge::renderer::scoped_block scoped_block(
		renderer_);

	postprocessing_.render_result();

	fruitlib::scenic::events::render event;

	node_base::forward_to_children(
		event);
}
