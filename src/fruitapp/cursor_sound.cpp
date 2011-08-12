#include <fcppt/chrono/milliseconds.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <sge/input/cursor/object.hpp>
#include <sge/input/cursor/position.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <sge/timer/basic.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fruitapp/cursor_sound.hpp>
#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>

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

fruitapp::cursor_sound::cursor_sound(
	fruitlib::scenic::optional_parent const &_parent,
	sge::input::cursor::object &_cursor,
	fruitapp::ingame_clock const &_clock,
	sge::renderer::device &_renderer,
	fruitlib::audio::sound_controller &_sound_controller)
:
	node_base(
		_parent),
	cursor_(
		_cursor),
	update_timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			fcppt::chrono::milliseconds(40))),
	cooldown_timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			fcppt::chrono::milliseconds(500))),
	renderer_(
		_renderer),
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


	typedef fcppt::math::vector::static_<float,2>::type vec2;

	sge::input::cursor::position new_pos =
		transform_position(
			cursor_.position(),
			renderer_.onscreen_target().viewport().get());

	float distance = fcppt::math::vector::length(
		fcppt::math::vector::structure_cast<vec2>(new_pos) -
		fcppt::math::vector::structure_cast<vec2>(last_pos_));

	float threshold = 0.3f * static_cast<float>(
		renderer_.onscreen_target().viewport().get().size().w());

	if (distance > threshold)
		sound_controller_.play(
			fruitlib::resource_tree::path("swing"));

	last_pos_ = new_pos;

	cooldown_timer_.reset();
}
