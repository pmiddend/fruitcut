#ifndef FRUITAPP_CURSOR_INSTANCE_SEQUENCE_HPP_INCLUDED
#define FRUITAPP_CURSOR_INSTANCE_SEQUENCE_HPP_INCLUDED

#include <fruitapp/cursor/instance_fwd.hpp>
#include <fruitapp/cursor/manager_fwd.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <sge/input/cursor/object_fwd.hpp>
#include <sge/line_drawer/line_sequence.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/optional/object_impl.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/config/external_begin.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace cursor
{
/**
\brief Wraps a collection of cursor instances and operations on all cursors.
*/
class instance_sequence
{
FCPPT_NONCOPYABLE(
	instance_sequence);
public:
	typedef
	std::vector
	<
		fcppt::unique_ptr
		<
			fruitapp::cursor::instance
		>
	>
	cursor_instance_sequence;

	instance_sequence(
		fruitapp::cursor::manager &,
		fruitlib::scenic::optional_parent const &);

	/// Is called by the manager when a new cursor is discovered after the instance sequence's creation
	void
	cursor_discover(
		sge::input::cursor::object &);

	/// Is called by the manager when a cursor is removed after the instance sequence's creation
	void
	cursor_remove(
		sge::input::cursor::object &);

	/// This is needed, for example, to calculate the intersection of all cursor trails with all fruits
	cursor_instance_sequence &
	cursors();

	/// Draw all the cursor's trails
	void
	draw_trails(
		sge::line_drawer::line_sequence &) const;

	~instance_sequence();
private:
	fruitapp::cursor::manager &manager_;
	fruitlib::scenic::optional_parent const parent_;
	cursor_instance_sequence instances_;
};
}
}

#endif
