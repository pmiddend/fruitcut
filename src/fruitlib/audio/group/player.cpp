#include <fruitlib/audio/group/player.hpp>
#include <fruitlib/audio/group/buffer.hpp>
#include <fruitlib/audio/group/sound_positional.hpp>
#include <fruitlib/exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/algorithm/ptr_container_erase.hpp>
#include <iostream>

fruitlib::audio::group::player::player(
	sge::audio::player &_impl,
	sge::audio::scalar const _gain,
	sge::audio::scalar const _pitch)
:
	impl_(
		_impl),
	gain_(
		_gain),
	pitch_(
		_pitch)
{
}

sge::audio::listener &
fruitlib::audio::group::player::listener()
{
	return 
		impl_.listener();
}

sge::audio::scalar 
fruitlib::audio::group::player::speed_of_sound() const
{
	return 
		impl_.speed_of_sound();
}

sge::audio::scalar 
fruitlib::audio::group::player::doppler_factor() const
{
	return 
		impl_.doppler_factor();
}

void 
fruitlib::audio::group::player::speed_of_sound(
	sge::audio::scalar const _speed_of_sound)
{
	return 
		impl_.speed_of_sound(
			_speed_of_sound);
}

void
fruitlib::audio::group::player::doppler_factor(
	sge::audio::scalar const _doppler_factor)
{
	return
		impl_.doppler_factor(
			_doppler_factor);
}

void
fruitlib::audio::group::player::gain(
	sge::audio::scalar const _gain)
{
	gain_ = 
		_gain;
	for(
		buffer_sequence::iterator i = 
			buffers_.begin();
		i != buffers_.end();
		++i)
		i->global_gain(
			_gain);

	for(
		sound_sequence::iterator i = 
			sounds_.begin();
		i != sounds_.end();
		++i)
		i->global_gain(
			_gain);
}

sge::audio::scalar
fruitlib::audio::group::player::gain() const
{
	return
		gain_;
}

void
fruitlib::audio::group::player::pitch(
	sge::audio::scalar const _pitch)
{
	pitch_ = 
		_pitch;
	for(
		buffer_sequence::iterator i = 
			buffers_.begin();
		i != buffers_.end();
		++i)
		i->global_pitch(
			_pitch);
}

sge::audio::scalar
fruitlib::audio::group::player::pitch() const
{
	return
		pitch_;
}

sge::audio::buffer_ptr const
fruitlib::audio::group::player::create_buffer(
	sge::audio::file &f)
{
	return 
		fcppt::make_shared_ptr<group::buffer>(
			fcppt::ref(
				*this),
			fcppt::ref(
				f),
			gain_,
			pitch_);
}

sge::audio::sound::positional_ptr const 
fruitlib::audio::group::player::create_positional_stream(
	sge::audio::file_ptr const f,
	sge::audio::sound::positional_parameters const &pp)
{
	return 
		fcppt::make_shared_ptr<group::sound_positional>(
			fcppt::ref(
				*this),
			impl_.create_positional_stream(
				f,
				pp),
			gain_,
			pitch_);
}

sge::audio::sound::base_ptr const 
fruitlib::audio::group::player::create_nonpositional_stream(
	sge::audio::file_ptr const f)
{
	return 
		fcppt::make_shared_ptr<group::sound_base>(
			fcppt::ref(
				*this),
			impl_.create_nonpositional_stream(
				f),
			gain_,
			pitch_);
}

sge::audio::player_capabilities_field const
fruitlib::audio::group::player::capabilities() const
{
	return 
		impl_.capabilities();
}

fruitlib::audio::group::player::~player()
{
}

void
fruitlib::audio::group::player::add_buffer(
	group::buffer &b)
{
	buffers_.push_back(
		&b);
}

void
fruitlib::audio::group::player::remove_buffer(
	group::buffer &b)
{
	FCPPT_ASSERT((
		fcppt::algorithm::ptr_container_erase(
			buffers_,
			&b)));
}

void
fruitlib::audio::group::player::add_sound(
	group::sound_base &b)
{
	sounds_.push_back(
		&b);
}

void
fruitlib::audio::group::player::remove_sound(
	group::sound_base &b)
{
	FCPPT_ASSERT((
		fcppt::algorithm::ptr_container_erase(
			sounds_,
			&b)));
}
