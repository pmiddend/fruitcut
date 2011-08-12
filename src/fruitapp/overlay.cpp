#include <fruitapp/overlay.hpp>
#include <fruitapp/postprocessing.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/renderer/viewport_size.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/image/colors.hpp>
#include <fcppt/math/dim/basic_impl.hpp>

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

	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::depth_func::off)
			(sge::renderer::state::bool_::clear_depth_buffer = false)
			(sge::renderer::state::bool_::clear_back_buffer = false));

	sge::renderer::scoped_block scoped_block(
		renderer_);

	postprocessing_.render_result();

	node_base::forward_to_children(
		fruitlib::scenic::events::render());
}
