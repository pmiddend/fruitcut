#ifndef FRUITAPP_CURSOR_INSTANCE_SEQUENCE_HPP_INCLUDED
#define FRUITAPP_CURSOR_INSTANCE_SEQUENCE_HPP_INCLUDED

#include <fruitapp/cursor/instance_fwd.hpp>
#include <fruitapp/cursor/manager_fwd.hpp>
#include <sge/line_drawer/line_sequence.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <sge/input/cursor/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/optional_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace cursor
{
class instance_sequence
{
FCPPT_NONCOPYABLE(
	instance_sequence);
public:
	typedef
	boost::ptr_vector<fruitapp::cursor::instance>
	cursor_instance_sequence;

	instance_sequence(
		fruitapp::cursor::manager &,
		fruitlib::scenic::optional_parent const &);

	void
	cursor_discover(
		sge::input::cursor::object &);

	void
	cursor_remove(
		sge::input::cursor::object &);

	cursor_instance_sequence &
	cursors();

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
