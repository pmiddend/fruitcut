#include "console.hpp"
#include <sge/console/gfx.hpp>

fruitcut::scenic::nodes::console::console(
	sge::console::gfx &_gfx)
:
	gfx_(
		_gfx)
{
}

fruitcut::scenic::nodes::console::~console()
{
}

void
fruitcut::scenic::nodes::console::update()
{
}

void
fruitcut::scenic::nodes::console::render()
{
	if(gfx_.active())
		gfx_.draw();
}
