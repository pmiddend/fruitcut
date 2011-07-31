#include "console.hpp"
#include "../../scenic/events/render.hpp"
#include <sge/console/gfx.hpp>

fruitcut::fruitlib::scenic::adaptors::console::console(
	scenic::optional_parent const &_parent,
	sge::console::gfx &_gfx)
:
	node_base(
		_parent),
	gfx_(
		_gfx)
{
}

fruitcut::fruitlib::scenic::adaptors::console::~console()
{
}

void
fruitcut::fruitlib::scenic::adaptors::console::react(
	events::render const &)
{
	if(gfx_.active())
		gfx_.draw();
}
