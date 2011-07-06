#include "player.hpp"
#include "buffer.hpp"
#include "../../exception.hpp"
#include <fcppt/text.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/algorithm/ptr_container_erase.hpp>
#include <iostream>

fruitcut::fruitlib::audio::group::player::player(
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
fruitcut::fruitlib::audio::group::player::listener()
{
	return 
		impl_.listener();
}

sge::audio::scalar 
fruitcut::fruitlib::audio::group::player::speed_of_sound() const
{
	return 
		impl_.speed_of_sound();
}

sge::audio::scalar 
fruitcut::fruitlib::audio::group::player::doppler_factor() const
{
	return 
		impl_.doppler_factor();
}

void 
fruitcut::fruitlib::audio::group::player::speed_of_sound(
	sge::audio::scalar const _speed_of_sound)
{
	return 
		impl_.speed_of_sound(
			_speed_of_sound);
}

void
fruitcut::fruitlib::audio::group::player::doppler_factor(
	sge::audio::scalar const _doppler_factor)
{
	return
		impl_.doppler_factor(
			_doppler_factor);
}

void
fruitcut::fruitlib::audio::group::player::gain(
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
}

sge::audio::scalar
fruitcut::fruitlib::audio::group::player::gain() const
{
	return
		gain_;
}

void
fruitcut::fruitlib::audio::group::player::pitch(
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
fruitcut::fruitlib::audio::group::player::pitch() const
{
	return
		pitch_;
}

sge::audio::buffer_ptr const
fruitcut::fruitlib::audio::group::player::create_buffer(
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
fruitcut::fruitlib::audio::group::player::create_positional_stream(
	sge::audio::file_ptr,
	sge::audio::sound::positional_parameters const &)
{
	throw 
		fruitlib::exception(
			FCPPT_TEXT("Streams are not supported"));
}

sge::audio::sound::base_ptr const 
fruitcut::fruitlib::audio::group::player::create_nonpositional_stream(
	sge::audio::file_ptr)
{
	throw 
		fruitlib::exception(
			FCPPT_TEXT("Streams are not supported"));
}

sge::audio::player_capabilities_field const
fruitcut::fruitlib::audio::group::player::capabilities() const
{
	return 
		impl_.capabilities();
}

fruitcut::fruitlib::audio::group::player::~player()
{
}

void
fruitcut::fruitlib::audio::group::player::add_buffer(
	group::buffer &b)
{
	buffers_.push_back(
		&b);
}

void
fruitcut::fruitlib::audio::group::player::remove_buffer(
	group::buffer &b)
{
	FCPPT_ASSERT((
		fcppt::algorithm::ptr_container_erase(
			buffers_,
			&b)));
}
