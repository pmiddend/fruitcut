#include "sound_base.hpp"
#include "player.hpp"
#include "buffer.hpp"
#include <iostream>

fruitlib::audio::group::sound_base::sound_base(
	group::buffer &_buffer,
	sge::audio::scalar const _global_gain,
	sge::audio::scalar const _global_pitch)
:
	player_(),
	buffer_(
		&_buffer),
	impl_(
		_buffer.impl_->create_nonpositional()),
	global_gain_(
		_global_gain),
	local_gain_(
		impl_->gain()),
	global_pitch_(
		_global_pitch),
	local_pitch_(
		impl_->pitch())
{
	gain(
		gain());
	pitch(
		pitch());
	_buffer.add_sound(
		*this);
}

fruitlib::audio::group::sound_base::sound_base(
	group::player &_player,
	sge::audio::sound::base_ptr const _impl,
	sge::audio::scalar const _global_gain,
	sge::audio::scalar const _global_pitch)
:
	player_(
		&_player),
	buffer_(
		0),
	impl_(
		_impl),
	global_gain_(
		_global_gain),
	local_gain_(
		impl_->gain()),
	global_pitch_(
		_global_pitch),
	local_pitch_(
		impl_->pitch())
{
	gain(
		gain());
	pitch(
		pitch());
	_player.add_sound(
		*this);
}

void 
fruitlib::audio::group::sound_base::play(
	sge::audio::sound::repeat::type const _repeat)
{
	return
		impl_->play(
			_repeat);
}

sge::audio::sound::repeat::type
fruitlib::audio::group::sound_base::repeat() const
{
	return
		impl_->repeat();
}

void 
fruitlib::audio::group::sound_base::toggle_pause()
{
	return
		impl_->toggle_pause();
}

sge::audio::sound::play_status::type 
fruitlib::audio::group::sound_base::status() const
{
	return
		impl_->status();
}

void 
fruitlib::audio::group::sound_base::gain(
	sge::audio::scalar const _local_gain)
{
	local_gain_ = 
		_local_gain;

	return
		impl_->gain(
			global_gain_ * local_gain_);
}

sge::audio::scalar 
fruitlib::audio::group::sound_base::gain() const
{
	return
		local_gain_;
}

void 
fruitlib::audio::group::sound_base::pitch(
	sge::audio::scalar const _local_pitch)
{
	local_pitch_ = 
		_local_pitch;

	return
		impl_->pitch(
			global_pitch_ * local_pitch_);
}

sge::audio::scalar 
fruitlib::audio::group::sound_base::pitch() const
{
	return
		local_pitch_;
}

void 
fruitlib::audio::group::sound_base::stop()
{
	return
		impl_->stop();
}

void
fruitlib::audio::group::sound_base::update()
{
	return
		impl_->update();
}

void
fruitlib::audio::group::sound_base::global_gain(
	sge::audio::scalar const _global_gain)
{
	global_gain_ = 
		_global_gain;
	gain(
		gain());
}

void
fruitlib::audio::group::sound_base::global_pitch(
	sge::audio::scalar const _global_pitch)
{
	global_pitch_ = 
		_global_pitch;
	pitch(
		pitch());
}

fruitlib::audio::group::sound_base::~sound_base()
{
	if(buffer_)
		buffer_->remove_sound(
			*this);
	else
		player_->remove_sound(
			*this);
}

fruitlib::audio::group::sound_base::sound_base(
	group::buffer &_buffer,
	sge::audio::sound::base_ptr const _impl,
	sge::audio::scalar const _global_gain,
	sge::audio::scalar const _global_pitch)
:
	player_(),
	buffer_(
		&_buffer),
	impl_(
		_impl),
	global_gain_(
		_global_gain),
	local_gain_(
		impl_->gain()),
	global_pitch_(
		_global_pitch),
	local_pitch_(
		impl_->pitch())
{
	gain(
		gain());
	pitch(
		pitch());
	_buffer.add_sound(
		*this);
}
