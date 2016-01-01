#include <fruitapp/cursor/sampler.hpp>
#include <sge/input/cursor/object.hpp>
#include <sge/input/cursor/position.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/target/base.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <fcppt/optional/maybe_void.hpp>
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

fruitapp::cursor::sampler::sampler(
	fruitlib::scenic::optional_parent const &_parent,
	sge::input::cursor::object &_cursor,
	fruitapp::ingame_clock const &_clock,
	fruitapp::ingame_clock::duration const &_update_frequency,
	sample_count const &_sample_count,
	sge::renderer::target::base const &_target)
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

fruitapp::cursor::sampler::position_buffer const &
fruitapp::cursor::sampler::positions() const
{
	return positions_;
}

void
fruitapp::cursor::sampler::clear()
{
	positions_.clear();
}

fruitapp::ingame_clock::duration const
fruitapp::cursor::sampler::total_expiry_duration() const
{
	return
		static_cast<fruitapp::ingame_clock::duration::rep>(
			sample_count_.get()) *
		update_timer_.interval();
}

fruitapp::cursor::sampler::~sampler()
{
}

void
fruitapp::cursor::sampler::react(
	fruitlib::scenic::events::update const &)
{
	if(
		!sge::timer::reset_when_expired(
			update_timer_))
		return;

	fcppt::optional::maybe_void(
		cursor_.position(),
		[
			this
		](
			sge::input::cursor::position const _position
		)
		{
			positions_.push_back(
				transform_position(
					_position,
					target_.viewport().get()));

			if(positions_.size() > sample_count_.get())
				positions_.pop_front();
		}
	);
}
