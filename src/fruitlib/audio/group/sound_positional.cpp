#include <fruitlib/audio/group/sound_positional.hpp>
#include <fruitlib/audio/group/buffer.hpp>
#include <sge/audio/buffer.hpp>

fruitlib::audio::group::sound_positional::sound_positional(
	group::buffer &_buffer,
	sge::audio::sound::positional_parameters const &_pp,
	sge::audio::scalar const _global_gain,
	sge::audio::scalar const _global_pitch)
:
	group::sound_base(
		_buffer,
		_buffer.impl_->create_positional(
			_pp),
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
	sge::audio::sound::positional_ptr const _impl,
	sge::audio::scalar const _global_gain,
	sge::audio::scalar const _global_pitch)
:
	group::sound_base(
		_player,
		_impl,
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
	sge::audio::sound::repeat::type const _repeat)
{
	return 
		sound_base::play(
			_repeat);
}

sge::audio::sound::repeat::type
fruitlib::audio::group::sound_positional::repeat() const
{
	return
		sound_base::repeat();
}

void 
fruitlib::audio::group::sound_positional::toggle_pause()
{
	return
		sound_base::toggle_pause();
}

sge::audio::sound::play_status::type 
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

sge::audio::scalar 
fruitlib::audio::group::sound_positional::gain() const
{
	return
		sound_base::gain();
}

void 
fruitlib::audio::group::sound_positional::pitch(
	sge::audio::scalar const _pitch)
{
	return
		sound_base::pitch(
			_pitch);
}

sge::audio::scalar 
fruitlib::audio::group::sound_positional::pitch() const
{
	return
		sound_base::pitch();
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

sge::audio::vector const 
fruitlib::audio::group::sound_positional::position() const
{
	return 
		impl_.position();
}

void 
fruitlib::audio::group::sound_positional::position(
	sge::audio::vector const &_position)
{
	return
		impl_.position(
			_position);
}

sge::audio::vector const 
fruitlib::audio::group::sound_positional::linear_velocity() const
{
	return
		impl_.linear_velocity();
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
fruitlib::audio::group::sound_positional::rolloff(
	sge::audio::scalar const _rolloff)
{
	return
		impl_.rolloff(
			_rolloff);
}

sge::audio::scalar 
fruitlib::audio::group::sound_positional::rolloff() const
{
	return
		impl_.rolloff();
}

void 
fruitlib::audio::group::sound_positional::direction(
	sge::audio::vector const &_direction)
{
	return
		impl_.direction(
			_direction);
}

sge::audio::vector const 
fruitlib::audio::group::sound_positional::direction() const
{
	return 
		impl_.direction();
}

void 
fruitlib::audio::group::sound_positional::inner_cone_angle(
	sge::audio::scalar const _inner_cone_angle)
{
	return
		impl_.inner_cone_angle(
			_inner_cone_angle);
}

sge::audio::scalar 
fruitlib::audio::group::sound_positional::inner_cone_angle() const
{
	return
		impl_.inner_cone_angle();
}

void 
fruitlib::audio::group::sound_positional::outer_cone_angle(
	sge::audio::scalar const _outer_cone_angle)
{
	return
		impl_.outer_cone_angle(
			_outer_cone_angle);
}

sge::audio::scalar 
fruitlib::audio::group::sound_positional::outer_cone_angle() const
{
	return 
		impl_.outer_cone_angle();
}
