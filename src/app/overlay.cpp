#include "overlay.hpp"
#include "../fruitlib/scenic/events/render.hpp"
#include <sge/renderer/viewport_size.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <fcppt/math/dim/basic_impl.hpp>

fruitcut::app::overlay::overlay(
	fruitlib::scenic::parent const &_parent,
	sge::renderer::device &_renderer)
:
	node_base(
		_parent),
	renderer_(
		_renderer)
{
}

fruitcut::app::overlay::~overlay()
{
}

void
fruitcut::app::overlay::react(
	fruitlib::scenic::events::render const &e)
{
	// Do we even have a viewport?
	if (!sge::renderer::viewport_size(renderer_).content())
		return;

	sge::renderer::scoped_block scoped_block(
		renderer_);

	node_base::forward_to_children(
		e);
}
