#include <fruitlib/scenic/adaptors/console.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/console/gfx.hpp>

fruitlib::scenic::adaptors::console::console(
	scenic::optional_parent const &_parent,
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
	events::render const &)
{
	if(gfx_.active())
		gfx_.render();
}
