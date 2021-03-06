#include <fruitapp/cursor/instance.hpp>
#include <fruitapp/cursor/instance_sequence.hpp>
#include <fruitapp/cursor/manager.hpp>
#include <fcppt/make_unique_ptr.hpp>


fruitapp::cursor::instance_sequence::instance_sequence(
	fruitapp::cursor::manager &_manager,
	fruitlib::scenic::optional_parent const &_parent)
:
	manager_(
		_manager),
	parent_(
		_parent),
	instances_()
{
}

void
fruitapp::cursor::instance_sequence::cursor_discover(
	sge::input::cursor::object &_new_cursor)
{
	instances_.push_back(
		fcppt::make_unique_ptr<fruitapp::cursor::instance>(
			manager_,
			_new_cursor,
			parent_));
}

void
fruitapp::cursor::instance_sequence::cursor_remove(
	sge::input::cursor::object &_old_cursor)
{
	for(
		cursor_instance_sequence::iterator it =
			instances_.begin();
		it != instances_.end();
		++it)
	{
		if(&((*it)->cursor_object()) == &_old_cursor)
		{
			instances_.erase(
				it);
			return;
		}
	}
}

fruitapp::cursor::instance_sequence::cursor_instance_sequence &
fruitapp::cursor::instance_sequence::cursors()
{
	return
		instances_;
}

void
fruitapp::cursor::instance_sequence::draw_trails(
	sge::line_drawer::line_sequence &_lines) const
{
	for(
		auto const &instance
		:
		instances_
	)
		instance->draw_trail(
			_lines);
}

fruitapp::cursor::instance_sequence::~instance_sequence()
{
	manager_.instance_sequence_destroyed(
		*this);
}
