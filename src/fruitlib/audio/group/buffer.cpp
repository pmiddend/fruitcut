#include "buffer.hpp"
#include "player.hpp"
#include "sound_base.hpp"
#include "sound_positional.hpp"
#include <fcppt/ref.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/algorithm/ptr_container_erase.hpp>
#include <iostream>

fruitcut::fruitlib::audio::group::buffer::buffer(
	group::player &_player,
	sge::audio::file &_file,
	sge::audio::scalar const _global_gain,
	sge::audio::scalar const _global_pitch)
:
	player_(
		_player),
	impl_(
		player_.impl_.create_buffer(
			_file)),
	global_gain_(
		_global_gain),
	global_pitch_(
		_global_pitch),
	sounds_()
{
	player_.add_buffer(
		*this);
}

sge::audio::sound::positional_ptr const
fruitcut::fruitlib::audio::group::buffer::create_positional(
	sge::audio::sound::positional_parameters const &p)
{
	return 
		fcppt::make_shared_ptr<group::sound_positional>(
			fcppt::ref(
				*this),
			p,
			global_gain_,
			global_pitch_);
}

sge::audio::sound::base_ptr const
fruitcut::fruitlib::audio::group::buffer::create_nonpositional()
{
	return 
		fcppt::make_shared_ptr<group::sound_base>(
			fcppt::ref(
				*this),
			global_gain_,
			global_pitch_);
}

void
fruitcut::fruitlib::audio::group::buffer::global_gain(
	sge::audio::scalar const _global_gain)
{
	global_gain_ = 
		_global_gain;

	for(
		sound_sequence::iterator i =
			sounds_.begin();
		i != sounds_.end();
		++i)
		i->global_gain(
			_global_gain);
}

void
fruitcut::fruitlib::audio::group::buffer::global_pitch(
	sge::audio::scalar const _global_pitch)
{
	global_pitch_ = 
		_global_pitch;

	for(
		sound_sequence::iterator i =
			sounds_.begin();
		i != sounds_.end();
		++i)
		i->global_pitch(
			_global_pitch);
}

fruitcut::fruitlib::audio::group::buffer::~buffer()
{
	player_.remove_buffer(
		*this);
}

void
fruitcut::fruitlib::audio::group::buffer::add_sound(
	group::sound_base &b)
{
	sounds_.push_back(
		&b);
}

void
fruitcut::fruitlib::audio::group::buffer::remove_sound(
	group::sound_base &b)
{
	FCPPT_ASSERT((
		fcppt::algorithm::ptr_container_erase(
			sounds_,
			&b)));
}