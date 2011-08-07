#include <fruitapp/cursor_trail.hpp>
#include <sge/input/cursor/object.hpp>
#include <sge/input/cursor/position.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/target_base.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/chrono/duration_arithmetic.hpp>
#include <iostream>

namespace
{
sge::input::cursor::position const
transform_position(
	sge::input::cursor::position const &p,
	sge::renderer::pixel_rect const &viewport)
{
	return 
		sge::input::cursor::position(
			static_cast<sge::input::cursor::position_unit>(
				p.x() - viewport.pos().x()),
			static_cast<sge::input::cursor::position_unit>(
				viewport.size().h() - p.y() + viewport.pos().y()));
}
}

fruitapp::cursor_trail::cursor_trail(
	fruitlib::scenic::optional_parent const &_parent,
	sge::input::cursor::object &_cursor,
	fruitapp::ingame_clock const &_clock,
	fruitapp::ingame_clock::duration const &_update_frequency,
	size_type const _sample_count,
	sge::renderer::target_base &_target)
:
	node_base(
		_parent),
	cursor_(
		_cursor),
	positions_(
		_sample_count),
	update_timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			_update_frequency)),
	target_(
		_target)
{
}

fruitapp::cursor_trail::position_buffer const &
fruitapp::cursor_trail::positions() const
{
	return positions_;
}

void
fruitapp::cursor_trail::clear()
{
	positions_.clear();
}

fruitapp::ingame_clock::duration const
fruitapp::cursor_trail::total_expiry_duration() const
{
	return 
		static_cast<fruitapp::ingame_clock::duration::rep>(
			positions_.capacity()) * 
		update_timer_.interval<fruitapp::ingame_clock::duration>();
}

fruitapp::cursor_trail::~cursor_trail()
{
}

void
fruitapp::cursor_trail::react(
	fruitlib::scenic::events::update const &)
{
	if(
		!sge::timer::reset_when_expired(
			update_timer_))
		return;

	positions_.push_back(
		transform_position(
			cursor_.position(),
			target_.viewport().get()));
}
