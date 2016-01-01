#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>
#include <fruitapp/cursor/sound.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/input/cursor/object.hpp>
#include <sge/input/cursor/optional_position.hpp>
#include <sge/input/cursor/position.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/timer/basic.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <fcppt/cast/int_to_float_fun.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/optional/maybe_void.hpp>
#include <fcppt/optional/maybe_void_multi.hpp>
#include <fcppt/config/external_begin.hpp>
#include <chrono>
#include <fcppt/config/external_end.hpp>


fruitapp::cursor::sound::sound(
	fruitlib::scenic::optional_parent const &_parent,
	sge::input::cursor::object &_cursor,
	fruitapp::ingame_clock const &_clock,
	fruitapp::viewport::manager &_viewport_manager,
	fruitlib::audio::sound_controller &_sound_controller)
:
	node_base(
		_parent),
	cursor_(
		_cursor),
	update_timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			std::chrono::milliseconds(40))),
	cooldown_timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			std::chrono::milliseconds(500))),
	viewport_manager_(
		_viewport_manager),
	sound_controller_(
		_sound_controller),
	last_pos_(
		cursor_.position())
{
}

fruitapp::cursor::sound::~sound()
{
}

void
fruitapp::cursor::sound::react(
	fruitlib::scenic::events::update const &)
{
	if(
		!cooldown_timer_.expired())
		return;

	if(
		!sge::timer::reset_when_expired(
			update_timer_))
		return;

	fcppt::optional::maybe_void(
		viewport_manager_.current_viewport(),
		[
			this
		](
			sge::renderer::target::viewport const &_viewport
		)
		{
			typedef fcppt::math::vector::static_<float,2> vec2;

			sge::input::cursor::optional_position const saved_last_pos(
				last_pos_);

			last_pos_ = cursor_.position();

			fcppt::optional::maybe_void_multi(
				[
					this,
					&_viewport
				](
					sge::input::cursor::position const _last_pos,
					sge::input::cursor::position const _saved_last_pos
				)
				{
					float distance = fcppt::math::vector::length(
						fcppt::math::vector::structure_cast<vec2, fcppt::cast::int_to_float_fun>(_last_pos) -
						fcppt::math::vector::structure_cast<vec2, fcppt::cast::int_to_float_fun>(_saved_last_pos));

					float threshold = 0.3f * static_cast<float>(
						_viewport.get().size().w());

					if (distance > threshold)
					{
						sound_controller_.play(
							fruitlib::resource_tree::path(FCPPT_TEXT("swing")));
						cooldown_timer_.reset();
					}
				},
				last_pos_,
				saved_last_pos
			);
		}
	);
}
