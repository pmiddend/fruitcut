#ifndef FRUITCUT_APP_CURSOR_TRAIL_HPP_INCLUDED
#define FRUITCUT_APP_CURSOR_TRAIL_HPP_INCLUDED

#include "../fruitlib/scenic/nodes/intrusive.hpp"
#include <sge/input/cursor/object_fwd.hpp>
#include <sge/input/cursor/position.hpp>
#include <sge/renderer/target_base_fwd.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/callback.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/circular_buffer.hpp>
#include <cstddef>

namespace fruitcut
{
namespace app
{
class cursor_trail
:
	public fruitlib::scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	cursor_trail);
public:
	typedef	
	boost::circular_buffer<sge::input::cursor::position>
	position_buffer;

	typedef
	position_buffer::size_type	
	size_type;

	explicit
	cursor_trail(
		sge::input::cursor::object &,
		sge::time::duration const &,
		sge::time::callback const &,
		size_type sample_count,
		sge::renderer::target_base &);

	position_buffer const &
	positions() const;

	void
	clear();

	// This is the time a cursor position sample has until it
	// expires. The fruit manager uses this to determine when to unlock
	// a fruit
	sge::time::duration const
	total_expiry_duration() const;

	~cursor_trail();
private:
	sge::input::cursor::object &cursor_;
	position_buffer positions_;
	sge::time::timer update_timer_;
	sge::renderer::target_base &target_;

	void
	update();

	void
	render();
};
}
}

#endif