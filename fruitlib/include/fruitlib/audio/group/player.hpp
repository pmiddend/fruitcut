#ifndef FRUITLIB_AUDIO_GROUP_PLAYER_HPP_INCLUDED
#define FRUITLIB_AUDIO_GROUP_PLAYER_HPP_INCLUDED

#include <fruitlib/audio/group/buffer_fwd.hpp>
#include <fruitlib/audio/group/sound_base_fwd.hpp>
#include <sge/audio/player.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/preprocessor/const.hpp>
#include <fcppt/preprocessor/pure.hpp>
#include <fcppt/preprocessor/warn_unused_result.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <fcppt/config/external_end.hpp>


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
	player(
		sge::audio::player &,
		sge::audio::scalar gain,
		sge::audio::scalar pitch);

	sge::audio::listener &
	listener()
	FCPPT_PP_CONST;

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
	gain() const
	FCPPT_PP_PURE;

	void
	pitch(
		sge::audio::scalar);

	sge::audio::scalar
	pitch() const
	FCPPT_PP_PURE;

	sge::audio::buffer_unique_ptr
	create_buffer(
		sge::audio::file &)
	FCPPT_PP_WARN_UNUSED_RESULT;

	sge::audio::sound::positional_unique_ptr
	create_positional_stream(
		sge::audio::file &,
		sge::audio::sound::positional_parameters const &)
	FCPPT_PP_WARN_UNUSED_RESULT;

	sge::audio::sound::base_unique_ptr
	create_nonpositional_stream(
		sge::audio::file &,
		sge::audio::sound::nonpositional_parameters const &)
	FCPPT_PP_WARN_UNUSED_RESULT;

	bool
	is_null() const
	FCPPT_PP_PURE;

	~player();
private:
	friend class group::buffer;
	friend class group::sound_base;

	typedef
	boost::ptr_vector<group::buffer,boost::view_clone_allocator>
	buffer_sequence;

	typedef
	boost::ptr_vector<group::sound_base,boost::view_clone_allocator>
	sound_sequence;

	sge::audio::player &impl_;
	buffer_sequence buffers_;
	sound_sequence sounds_;
	sge::audio::scalar gain_,pitch_;

	void
	add_buffer(
		group::buffer &);

	void
	remove_buffer(
		group::buffer &);

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
