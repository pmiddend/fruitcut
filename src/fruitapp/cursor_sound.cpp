#include <fruitapp/cursor_sound.hpp>
#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/input/cursor/object.hpp>
#include <sge/input/cursor/position.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/timer/basic.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/duration.hpp>
#include <fcppt/config/external_end.hpp>


fruitapp::cursor_sound::cursor_sound(
	fruitlib::scenic::optional_parent const &_parent,
	sge::input::cursor::object &_cursor,
	fruitapp::ingame_clock const &_clock,
	fruitapp::viewport::manager const &_viewport_manager,
	fruitlib::audio::sound_controller &_sound_controller)
:
	node_base(
		_parent),
	cursor_(
		_cursor),
	update_timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			boost::chrono::milliseconds(40))),
	cooldown_timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			boost::chrono::milliseconds(500))),
	viewport_manager_(
		_viewport_manager),
	sound_controller_(
		_sound_controller),
	last_pos_()
{
}

fruitapp::cursor_sound::~cursor_sound()
{
}

void
fruitapp::cursor_sound::react(
	fruitlib::scenic::events::update const &)
{
	if(
		!cooldown_timer_.expired())
		return;

	if(
		!sge::timer::reset_when_expired(
			update_timer_))
		return;

	if(!viewport_manager_.current_viewport())
		return;

	typedef fcppt::math::vector::static_<float,2>::type vec2;

	if(!cursor_.position())
		return;

	sge::input::cursor::position new_pos =
			*cursor_.position();

	float distance = fcppt::math::vector::length(
		fcppt::math::vector::structure_cast<vec2>(new_pos) -
		fcppt::math::vector::structure_cast<vec2>(last_pos_));

	float threshold = 0.3f * static_cast<float>(
		viewport_manager_.current_viewport()->get().size().w());

	if (distance > threshold)
	{
		sound_controller_.play(
			fruitlib::resource_tree::path(FCPPT_TEXT("swing")));
		cooldown_timer_.reset();
	}

	last_pos_ = new_pos;

}
