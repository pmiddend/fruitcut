#ifndef FRUITCUT_FRUITLIB_AUDIO_GROUP_PLAYER_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_AUDIO_GROUP_PLAYER_HPP_INCLUDED

#include "buffer_fwd.hpp"
#include <sge/audio/player.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/container/bitfield/bitfield.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace audio
{
namespace group
{
class player
:
	public sge::audio::player
{
FCPPT_NONCOPYABLE(
	player);
public:
	explicit
	player(
		sge::audio::player &,
		sge::audio::scalar gain,
		sge::audio::scalar pitch);

	sge::audio::listener &
	listener();

	sge::audio::scalar 
	speed_of_sound() const;

	sge::audio::scalar 
	doppler_factor() const;

	void 
	speed_of_sound(
		sge::audio::scalar);

	void
	doppler_factor(
		sge::audio::scalar);

	void
	gain(
		sge::audio::scalar);

	sge::audio::scalar
	gain() const;

	void
	pitch(
		sge::audio::scalar);

	sge::audio::scalar
	pitch() const;

	sge::audio::buffer_ptr const
	create_buffer(
		sge::audio::file &);

	sge::audio::sound::positional_ptr const 
	create_positional_stream(
		sge::audio::file_ptr,
		sge::audio::sound::positional_parameters const &);

	sge::audio::sound::base_ptr const 
	create_nonpositional_stream(
		sge::audio::file_ptr);

	sge::audio::player_capabilities_field const
	capabilities() const;

	~player();
private:
	friend class group::buffer;

	typedef
	boost::ptr_vector<group::buffer,boost::view_clone_allocator>
	buffer_sequence;

	sge::audio::player &impl_;
	buffer_sequence buffers_;
	sge::audio::scalar gain_,pitch_;

	void
	add_buffer(
		group::buffer &);

	void
	remove_buffer(
		group::buffer &);
};
}
}
}
}

#endif
