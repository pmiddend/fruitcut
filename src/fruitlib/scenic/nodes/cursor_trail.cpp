#include "cursor_trail.hpp"
#include "../../cursor_trail.hpp"

fruitcut::fruitlib::scenic::nodes::cursor_trail::cursor_trail(
	fruitlib::cursor_trail &_cursor_trail)
:
	cursor_trail_(
		_cursor_trail)
{
}

fruitcut::fruitlib::scenic::nodes::cursor_trail::~cursor_trail()
{
}

void
fruitcut::fruitlib::scenic::nodes::cursor_trail::update()
{
	cursor_trail_.update();
}

void
fruitcut::fruitlib::scenic::nodes::cursor_trail::render()
{
}
