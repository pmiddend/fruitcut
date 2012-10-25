#include <fruitlib/audio/group/buffer.hpp>
#include <fruitlib/audio/group/player.hpp>
#include <fruitlib/audio/group/sound_base.hpp>
#include <fruitlib/audio/group/sound_positional.hpp>
#include <sge/audio/sound/positional_parameters.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/algorithm/ptr_container_erase.hpp>
#include <fcppt/assert/error.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitlib::audio::group::buffer::buffer(
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

sge::audio::sound::positional_unique_ptr
fruitlib::audio::group::buffer::create_positional(
	sge::audio::sound::positional_parameters const &p)
{
	return
		sge::audio::sound::positional_unique_ptr(
			fcppt::make_unique_ptr<group::sound_positional>(
				fcppt::ref(
					*this),
				p,
				global_gain_,
				global_pitch_));
}

sge::audio::sound::base_unique_ptr
fruitlib::audio::group::buffer::create_nonpositional(
	sge::audio::sound::nonpositional_parameters const &p)
{
	return
		sge::audio::sound::base_unique_ptr(
			fcppt::make_unique_ptr<group::sound_base>(
				fcppt::ref(
					*this),
				fcppt::cref(
					p),
				global_gain_,
				global_pitch_));
}

void
fruitlib::audio::group::buffer::global_gain(
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
fruitlib::audio::group::buffer::global_pitch(
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

fruitlib::audio::group::buffer::~buffer()
{
	player_.remove_buffer(
		*this);
}

void
fruitlib::audio::group::buffer::add_sound(
	group::sound_base &b)
{
	sounds_.push_back(
		&b);
}

void
fruitlib::audio::group::buffer::remove_sound(
	group::sound_base &b)
{
	FCPPT_ASSERT_ERROR((
		fcppt::algorithm::ptr_container_erase(
			sounds_,
			&b)));
}