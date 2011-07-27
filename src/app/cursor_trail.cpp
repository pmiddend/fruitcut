#include "cursor_trail.hpp"
#include <sge/input/cursor/object.hpp>
#include <sge/input/cursor/position.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/target_base.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <sge/timer/parameters.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/chrono/duration.hpp>
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

fruitcut::app::cursor_trail::cursor_trail(
	sge::input::cursor::object &_cursor,
	sge::time::duration const &_update_frequency,
	sge::time::callback const &_callback,
	size_type const _sample_count,
	sge::renderer::target_base &_target)
:
	fruitlib::scenic::nodes::intrusive(),
	cursor_(
		_cursor),
	positions_(
		_sample_count),
	update_timer_(
		_update_frequency,
		sge::time::activation_state::active,
		_callback),
	target_(
		_target)
{
}

fruitcut::app::cursor_trail::position_buffer const &
fruitcut::app::cursor_trail::positions() const
{
	return positions_;
}

void
fruitcut::app::cursor_trail::clear()
{
	positions_.clear();
}

sge::time::duration const
fruitcut::app::cursor_trail::total_expiry_duration() const
{
	return 
		sge::time::duration(
			static_cast<sge::time::duration::rep>(
				update_timer_.interval() * 
				static_cast<sge::time::duration::rep>(
					positions_.capacity())));
}

fruitcut::app::cursor_trail::~cursor_trail()
{
}

void
fruitcut::app::cursor_trail::update()
{
	if(
		!update_timer_.update_b())
		return;

	positions_.push_back(
		transform_position(
			cursor_.position(),
			target_.viewport().get()));
}

void
fruitcut::app::cursor_trail::render()
{
}