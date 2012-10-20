#ifndef FRUITCUT_APP_CURSOR_TRAIL_HPP_INCLUDED
#define FRUITCUT_APP_CURSOR_TRAIL_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/input/cursor/object_fwd.hpp>
#include <sge/input/cursor/position.hpp>
#include <sge/renderer/target/base_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <cstddef>
#include <deque>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
class cursor_trail
:
	public fruitlib::scenic::node<cursor_trail>
{
FCPPT_NONCOPYABLE(
	cursor_trail);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	typedef
	std::deque<sge::input::cursor::position>
	position_buffer;

	typedef
	position_buffer::size_type
	size_type;

	cursor_trail(
		fruitlib::scenic::optional_parent const &,
		sge::input::cursor::object &,
		fruitapp::ingame_clock const &,
		fruitapp::ingame_clock::duration const &update_duration,
		size_type sample_count,
		sge::renderer::target::base &);

	position_buffer const &
	positions() const;

	void
	clear();

	// This is the time a cursor position sample has until it
	// expires. The fruit manager uses this to determine when to unlock
	// a fruit
	fruitapp::ingame_clock::duration const
	total_expiry_duration() const;

	~cursor_trail();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	sge::input::cursor::object &cursor_;
	sge::renderer::target::base &target_;
	position_buffer positions_;
	fruitapp::ingame_timer update_timer_;
	size_type sample_count_;

};
}

#endif
