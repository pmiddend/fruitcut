#ifndef FRUITLIB_AUDIO_GROUP_SOUND_POSITIONAL_HPP_INCLUDED
#define FRUITLIB_AUDIO_GROUP_SOUND_POSITIONAL_HPP_INCLUDED

#include <fruitlib/audio/group/buffer_fwd.hpp>
#include <fruitlib/audio/group/player_fwd.hpp>
#include <fruitlib/audio/group/sound_base.hpp>
#include <sge/audio/sound/play_status_fwd.hpp>
#include <sge/audio/sound/positional.hpp>
#include <sge/audio/sound/positional_parameters.hpp>
#include <sge/audio/sound/positional_unique_ptr.hpp>
#include <sge/audio/sound/repeat_fwd.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitlib
{
namespace audio
{
namespace group
{
class sound_positional
:
	public group::sound_base,
	public sge::audio::sound::positional
{
FCPPT_NONCOPYABLE(
	sound_positional);
public:
	sound_positional(
		group::buffer &,
		sge::audio::sound::positional_parameters const &,
		sge::audio::scalar gain,
		sge::audio::scalar pitch);

	sound_positional(
		group::player &,
		sge::audio::sound::positional_unique_ptr,
		sge::audio::scalar gain,
		sge::audio::scalar pitch);

	void
	play(
		sge::audio::sound::repeat
	)
	override;

	void
	toggle_pause()
	override;

	sge::audio::sound::play_status
	status() const
	override;

	void
	gain(
		sge::audio::scalar
	)
	override;

	void
	pitch(
		sge::audio::scalar
	)
	override;

	void
	stop()
	override;

	void
	update()
	override;

	void
	position(
		sge::audio::vector const &
	)
	override;

	void
	linear_velocity(
		sge::audio::vector const &
	)
	override;

	void
	rolloff_factor(
		sge::audio::scalar
	)
	override;

	void
	reference_distance(
		sge::audio::scalar
	)
	override;

	void
	max_distance(
		sge::audio::scalar
	)
	override;

	void
	direction(
		sge::audio::sound::optional_direction const &
	)
	override;

	void
	inner_cone_angle(
		sge::audio::scalar
	)
	override;

	void
	outer_cone_angle(
		sge::audio::scalar
	)
	override;

	void
	outer_cone_gain(
		sge::audio::scalar
	)
	override;
private:
	sge::audio::sound::positional &impl_;
};
}
}
}

#endif
