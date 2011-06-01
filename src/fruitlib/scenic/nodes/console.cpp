#include "console.hpp"
#include <sge/console/gfx.hpp>

fruitcut::fruitlib::scenic::nodes::console::console(
	sge::console::gfx &_gfx)
:
	gfx_(
		_gfx)
{
}

fruitcut::fruitlib::scenic::nodes::console::~console()
{
}

void
fruitcut::fruitlib::scenic::nodes::console::update()
{
}

void
fruitcut::fruitlib::scenic::nodes::console::render()
{
	if(gfx_.active())
		gfx_.draw();
}
