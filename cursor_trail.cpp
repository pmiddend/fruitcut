#include "cursor_trail.hpp"
#include <sge/input/cursor/object.hpp>
#include <sge/input/cursor/position.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <sge/renderer/target_base.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/vector/output.hpp>
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
				viewport.dimension().h() - p.y() + viewport.pos().y()));
}
}

fruitcut::cursor_trail::cursor_trail(
	sge::input::cursor::object &_cursor,
	sge::time::duration const &_update_frequency,
	size_type const _sample_count,
	sge::renderer::target_base_ptr const _target)
:
	cursor_(
		_cursor),
	positions_(
		_sample_count),
	update_timer_(
		_update_frequency),
	target_(
		_target)
{
}

void
fruitcut::cursor_trail::update()
{
	if (!update_timer_.update_b())
		return;

	std::cout << "adding new position: " << transform_position(
			cursor_.position(),
			target_->viewport().get()) << "\n";

	positions_.push_back(
		transform_position(
			cursor_.position(),
			target_->viewport().get()));
}

fruitcut::cursor_trail::position_buffer const &
fruitcut::cursor_trail::positions() const
{
	return positions_;
}

void
fruitcut::cursor_trail::clear()
{
	positions_.clear();
}

fruitcut::cursor_trail::~cursor_trail()
{
}
