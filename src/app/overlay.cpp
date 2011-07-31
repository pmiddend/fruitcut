#include "overlay.hpp"
#include "postprocessing.hpp"
#include "../fruitlib/scenic/events/render.hpp"
#include "../fruitlib/scenic/events/update.hpp"
#include <sge/renderer/viewport_size.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <fcppt/math/dim/basic_impl.hpp>

fruitcut::app::overlay::overlay(
	fruitlib::scenic::parent const &_parent,
	sge::renderer::device &_renderer,
	app::postprocessing &_postprocessing)
:
	node_base(
		_parent),
	renderer_(
		_renderer),
	postprocessing_(
		_postprocessing)
{
}

fruitcut::app::overlay::~overlay()
{
}

void
fruitcut::app::overlay::react(
	fruitlib::scenic::events::update const &e)
{
	// Update my children
	node_base::forward_to_children(
		e);

	// Do we even have a viewport?
	if (!sge::renderer::viewport_size(renderer_).content())
		return;

	sge::renderer::scoped_block scoped_block(
		renderer_);

	postprocessing_.render_result();

	node_base::forward_to_children(
		fruitlib::scenic::events::render());
}
