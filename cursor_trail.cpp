#include "cursor_trail.hpp"
#include <sge/input/cursor/object.hpp>
#include <iostream>

fruitcut::cursor_trail::cursor_trail(
	sge::input::cursor::object &_cursor,
	sge::time::duration const &_update_frequency,
	size_type const _sample_count)
:
	cursor_(
		_cursor),
	positions_(
		_sample_count),
	update_timer_(
		_update_frequency)
{
}

void
fruitcut::cursor_trail::update()
{
	if (!update_timer_.update_b())
		return;
	
	positions_.push_back(
		cursor_.position());
}

fruitcut::cursor_trail::position_buffer const &
fruitcut::cursor_trail::positions() const
{
	return positions_;
}

fruitcut::cursor_trail::~cursor_trail()
{
}
