#ifndef FRUITLIB_AUDIO_GROUP_SOUND_BASE_HPP_INCLUDED
#define FRUITLIB_AUDIO_GROUP_SOUND_BASE_HPP_INCLUDED

#include <fruitlib/audio/group/buffer_fwd.hpp>
#include <fruitlib/audio/group/player_fwd.hpp>
#include <sge/audio/sound/base.hpp>
#include <sge/audio/sound/base_unique_ptr.hpp>
#include <sge/audio/sound/base_unique_ptr.hpp>
#include <sge/audio/sound/nonpositional_parameters_fwd.hpp>
#include <sge/audio/sound/play_status_fwd.hpp>
#include <sge/audio/sound/repeat_fwd.hpp>
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
	sound_base(
		group::buffer &,
		sge::audio::sound::nonpositional_parameters const &,
		sge::audio::scalar global_gain,
		sge::audio::scalar global_pitch);

	sound_base(
		group::player &,
		sge::audio::sound::base_unique_ptr,
		sge::audio::scalar global_gain,
		sge::audio::scalar global_pitch);

	void
	play(
		sge::audio::sound::repeat);

	void
	toggle_pause();

	sge::audio::sound::play_status
	status() const;

	void
	gain(
		sge::audio::scalar);

	void
	pitch(
		sge::audio::scalar);

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
		sge::audio::sound::base_unique_ptr,
		sge::audio::scalar gain,
		sge::audio::scalar pitch);
protected:
	group::player *player_;
	group::buffer *buffer_;
	sge::audio::sound::base_unique_ptr const impl_;
	sge::audio::scalar global_gain_;
	sge::audio::scalar local_gain_;
	sge::audio::scalar global_pitch_;
	sge::audio::scalar local_pitch_;
};
}
}
}

#endif
