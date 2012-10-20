#include <fruitlib/scenic/adaptors/console.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/console/gfx.hpp>


fruitlib::scenic::adaptors::console::console(
	fruitlib::scenic::optional_parent const &_parent,
	sge::console::gfx &_gfx)
:
	node_base(
		_parent),
	gfx_(
		_gfx)
{
}

fruitlib::scenic::adaptors::console::~console()
{
}

void
fruitlib::scenic::adaptors::console::react(
	fruitlib::scenic::events::render const &_render_event)
{
	if(gfx_.active())
		gfx_.render(
			_render_event.context());
}
