#include <fruitapp/cursor_trail.hpp>
#include <sge/input/cursor/object.hpp>
#include <sge/input/cursor/position.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/target/base.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <fcppt/assert/pre.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/structure_cast.hpp>


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
	sge::renderer::target::base &_target)
:
	node_base(
		_parent),
	cursor_(
		_cursor),
	target_(
		_target),
	positions_(),
	update_timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			_update_frequency)),
	sample_count_(
		_sample_count)
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
			sample_count_) *
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

	if(
		!cursor_.position())
		return;

	positions_.push_back(
		transform_position(
			*cursor_.position(),
			target_.viewport().get()));

	if(positions_.size() > sample_count_)
		positions_.pop_front();
}
