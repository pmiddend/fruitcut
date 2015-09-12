#include <fruitlib/exception.hpp>
#include <fruitlib/audio/group/buffer.hpp>
#include <fruitlib/audio/group/player.hpp>
#include <fruitlib/audio/group/sound_positional.hpp>
#include <sge/audio/buffer.hpp>
#include <sge/audio/sound/base.hpp>
#include <sge/audio/sound/positional.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/unique_ptr_to_base.hpp>
#include <fcppt/assert/error.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitlib::audio::group::player::player(
	sge::audio::player &_impl,
	sge::audio::scalar const _gain,
	sge::audio::scalar const _pitch)
:
	impl_(
		_impl),
	buffers_(),
	sounds_(),
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
		auto const buffer
		:
		buffers_
	)
		buffer->global_gain(
			_gain);

	for(
		auto const sound
		:
		sounds_
	)
		sound->global_gain(
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
		auto const buffer
		:
		buffers_
	)
		buffer->global_pitch(
			_pitch);
}

sge::audio::scalar
fruitlib::audio::group::player::pitch() const
{
	return
		pitch_;
}

sge::audio::buffer_unique_ptr
fruitlib::audio::group::player::create_buffer(
	sge::audio::file &f)
{
	return
		fcppt::unique_ptr_to_base<
			sge::audio::buffer
		>(
			fcppt::make_unique_ptr<group::buffer>(
				*this,
				f,
				gain_,
				pitch_));
}

sge::audio::sound::positional_unique_ptr
fruitlib::audio::group::player::create_positional_stream(
	sge::audio::file &f,
	sge::audio::sound::positional_parameters const &pp)
{
	return
		fcppt::unique_ptr_to_base<
			sge::audio::sound::positional
		>(
			fcppt::make_unique_ptr<group::sound_positional>(
				*this,
				impl_.create_positional_stream(
					f,
					pp),
				gain_,
				pitch_));
}

sge::audio::sound::base_unique_ptr
fruitlib::audio::group::player::create_nonpositional_stream(
	sge::audio::file &f,
	sge::audio::sound::nonpositional_parameters const &p)
{
	return
		fcppt::unique_ptr_to_base<
			sge::audio::sound::base
		>(
			fcppt::make_unique_ptr<group::sound_base>(
				*this,
				impl_.create_nonpositional_stream(
					f,
					p),
				gain_,
				pitch_));
}

bool
fruitlib::audio::group::player::is_null() const
{
	return
		impl_.is_null();
}

fruitlib::audio::group::player::~player()
{
}

void
fruitlib::audio::group::player::add_buffer(
	group::buffer &b)
{
	buffers_.insert(
		&b);
}

void
fruitlib::audio::group::player::remove_buffer(
	group::buffer &b)
{
	FCPPT_ASSERT_ERROR(
		buffers_.erase(
			&b));
}

void
fruitlib::audio::group::player::add_sound(
	group::sound_base &b)
{
	sounds_.insert(
		&b);
}

void
fruitlib::audio::group::player::remove_sound(
	group::sound_base &b)
{
	FCPPT_ASSERT_ERROR(
		sounds_.erase(
			&b));
}
