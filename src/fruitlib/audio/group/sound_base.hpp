#ifndef FRUITLIB_AUDIO_GROUP_SOUND_BASE_HPP_INCLUDED
#define FRUITLIB_AUDIO_GROUP_SOUND_BASE_HPP_INCLUDED

#include "buffer_fwd.hpp"
#include "player_fwd.hpp"
#include <sge/audio/audio.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitlib
{
namespace audio
{
namespace group
{
class sound_base
:
	public sge::audio::sound::base
{
FCPPT_NONCOPYABLE(
	sound_base);
public:
	explicit
	sound_base(
		group::buffer &,
		sge::audio::scalar global_gain,
		sge::audio::scalar global_pitch);

	explicit
	sound_base(
		group::player &,
		sge::audio::sound::base_ptr,
		sge::audio::scalar global_gain,
		sge::audio::scalar global_pitch);
	
	void 
	play(
		sge::audio::sound::repeat::type);

	sge::audio::sound::repeat::type
	repeat() const;

	void 
	toggle_pause();

	sge::audio::sound::play_status::type 
	status() const;

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

	void 
	stop();

	void
	update();

	void
	global_gain(
		sge::audio::scalar);

	void
	global_pitch(
		sge::audio::scalar);

	~sound_base();
protected:
	explicit
	sound_base(
		group::buffer &,
		sge::audio::sound::base_ptr,
		sge::audio::scalar gain,
		sge::audio::scalar pitch);
protected:
	group::player *player_;
	group::buffer *buffer_;
	sge::audio::sound::base_ptr impl_;
	sge::audio::scalar global_gain_;
	sge::audio::scalar local_gain_;
	sge::audio::scalar global_pitch_;
	sge::audio::scalar local_pitch_;
};
}
}
}

#endif
