#include "cursor_trail.hpp"
#include "../../cursor_trail.hpp"

fruitcut::scenic::nodes::cursor_trail::cursor_trail(
	fruitcut::cursor_trail &_cursor_trail)
:
	cursor_trail_(
		_cursor_trail)
{
}

fruitcut::scenic::nodes::cursor_trail::~cursor_trail()
{
}

void
fruitcut::scenic::nodes::cursor_trail::update()
{
	cursor_trail_.update();
}

void
fruitcut::scenic::nodes::cursor_trail::render()
{
}
