#include <fruitlib/audio/group/buffer.hpp>
#include <fruitlib/audio/group/sound_positional.hpp>
#include <sge/audio/buffer.hpp>
#include <sge/audio/sound/base.hpp>
#include <sge/audio/sound/play_status.hpp>
#include <sge/audio/sound/repeat.hpp>
#include <fcppt/unique_ptr_to_base.hpp>
#include <fcppt/config/external_begin.hpp>
#include <utility>
#include <fcppt/config/external_end.hpp>


fruitlib::audio::group::sound_positional::sound_positional(
	group::buffer &_buffer,
	sge::audio::sound::positional_parameters const &_pp,
	sge::audio::scalar const _global_gain,
	sge::audio::scalar const _global_pitch)
:
	group::sound_base(
		_buffer,
		fcppt::unique_ptr_to_base<
			sge::audio::sound::base
		>(
			_buffer.impl_->create_positional(
				_pp)),
		_global_gain,
		_global_pitch),
	sge::audio::sound::positional(),
	impl_(
		dynamic_cast<sge::audio::sound::positional &>(
			*sound_base::impl_))
{
}

fruitlib::audio::group::sound_positional::sound_positional(
	group::player &_player,
	sge::audio::sound::positional_unique_ptr _impl,
	sge::audio::scalar const _global_gain,
	sge::audio::scalar const _global_pitch)
:
	group::sound_base(
		_player,
		fcppt::unique_ptr_to_base<
			sge::audio::sound::base
		>(
			std::move(
				_impl)),
		_global_gain,
		_global_pitch),
	sge::audio::sound::positional(),
	impl_(
		dynamic_cast<sge::audio::sound::positional &>(
			*sound_base::impl_))
{
}

void
fruitlib::audio::group::sound_positional::play(
	sge::audio::sound::repeat const _repeat)
{
	return
		sound_base::play(
			_repeat);
}

void
fruitlib::audio::group::sound_positional::toggle_pause()
{
	return
		sound_base::toggle_pause();
}

sge::audio::sound::play_status
fruitlib::audio::group::sound_positional::status() const
{
	return
		sound_base::status();
}

void
fruitlib::audio::group::sound_positional::gain(
	sge::audio::scalar const _gain)
{
	return
		sound_base::gain(
			_gain);
}

void
fruitlib::audio::group::sound_positional::pitch(
	sge::audio::scalar const _pitch)
{
	return
		sound_base::pitch(
			_pitch);
}

void
fruitlib::audio::group::sound_positional::stop()
{
	return
		sound_base::stop();
}

void
fruitlib::audio::group::sound_positional::update()
{
	return
		sound_base::update();
}

void
fruitlib::audio::group::sound_positional::position(
	sge::audio::vector const &_position)
{
	return
		impl_.position(
			_position);
}

void
fruitlib::audio::group::sound_positional::linear_velocity(
	sge::audio::vector const &_linear_velocity)
{
	return
		impl_.linear_velocity(
			_linear_velocity);
}

void
fruitlib::audio::group::sound_positional::rolloff_factor(
	sge::audio::scalar const _rolloff)
{
	return
		impl_.rolloff_factor(
			_rolloff);
}

void
fruitlib::audio::group::sound_positional::reference_distance(
	sge::audio::scalar const _reference_distance)
{
	return
		impl_.reference_distance(
			_reference_distance);
}

void
fruitlib::audio::group::sound_positional::max_distance(
	sge::audio::scalar const _max_distance)
{
	return
		impl_.max_distance(
			_max_distance);
}

void
fruitlib::audio::group::sound_positional::direction(
	sge::audio::sound::optional_direction const &_direction)
{
	return
		impl_.direction(
			_direction);
}

void
fruitlib::audio::group::sound_positional::inner_cone_angle(
	sge::audio::scalar const _inner_cone_angle)
{
	return
		impl_.inner_cone_angle(
			_inner_cone_angle);
}

void
fruitlib::audio::group::sound_positional::outer_cone_angle(
	sge::audio::scalar const _outer_cone_angle)
{
	return
		impl_.outer_cone_angle(
			_outer_cone_angle);
}

void
fruitlib::audio::group::sound_positional::outer_cone_gain(
	sge::audio::scalar const _outer_cone_gain)
{
	return
		impl_.outer_cone_gain(
			_outer_cone_gain);
}
