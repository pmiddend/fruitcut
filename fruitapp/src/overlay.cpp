#include <fruitapp/overlay.hpp>
#include <fruitapp/postprocessing/subsystems/main.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/image/color/predef.hpp>
#include <sge/renderer/clear/parameters.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/context/scoped_ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/target/onscreen.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/dim/contents.hpp>
#include <fcppt/math/dim/object_impl.hpp>


fruitapp::overlay::overlay(
	fruitlib::scenic::optional_parent const &_parent,
	sge::renderer::device::ffp &_renderer,
	fruitapp::postprocessing::subsystems::main &_postprocessing)
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
	if(
		fcppt::math::dim::contents(
			renderer_.onscreen_target().viewport().get().size()
		)
		==
		0
	)
		return;

	renderer_.onscreen_target().clear(
		sge::renderer::clear::parameters()
			.back_buffer(
				sge::image::color::predef::black())
			.depth_buffer(
				sge::renderer::clear::depth_buffer_value(
					1.0f)));

	sge::renderer::context::scoped_ffp scoped_context(
		renderer_,
		renderer_.onscreen_target());

	postprocessing_.render_result(
		scoped_context.get());

	fruitlib::scenic::events::render event(
		scoped_context.get());

	node_base::forward_to_children(
		event);
}
