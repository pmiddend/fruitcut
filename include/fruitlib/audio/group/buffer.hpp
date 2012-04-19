#ifndef FRUITLIB_AUDIO_GROUP_BUFFER_HPP_INCLUDED
#define FRUITLIB_AUDIO_GROUP_BUFFER_HPP_INCLUDED

#include <fruitlib/audio/group/player_fwd.hpp>
#include <fruitlib/audio/group/sound_base_fwd.hpp>
#include <fruitlib/audio/group/sound_positional_fwd.hpp>
#include <sge/audio/buffer.hpp>
#include <sge/audio/buffer_scoped_ptr.hpp>
#include <sge/audio/file_fwd.hpp>
#include <sge/audio/scalar.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace audio
{
namespace group
{
class buffer
:
	public sge::audio::buffer
{
FCPPT_NONCOPYABLE(
	buffer);
public:
	explicit
	buffer(
		group::player &,
		sge::audio::file &,
		sge::audio::scalar gain,
		sge::audio::scalar pitch);

	sge::audio::sound::positional_unique_ptr
	create_positional(
		sge::audio::sound::positional_parameters const &);

	sge::audio::sound::base_unique_ptr
	create_nonpositional(
		sge::audio::sound::nonpositional_parameters const &);

	void
	global_gain(
		sge::audio::scalar);

	void
	global_pitch(
		sge::audio::scalar);

	~buffer();
private:
	friend class group::sound_base;
	friend class group::sound_positional;

	typedef
	boost::ptr_vector<group::sound_base,boost::view_clone_allocator>
	sound_sequence;

	group::player &player_;
	sge::audio::buffer_scoped_ptr impl_;
	sge::audio::scalar global_gain_,global_pitch_;
	sound_sequence sounds_;

	void
	add_sound(
		group::sound_base &);

	void
	remove_sound(
		group::sound_base &);
};
}
}
}

#endif
