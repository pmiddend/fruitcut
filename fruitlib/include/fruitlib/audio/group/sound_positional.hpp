#ifndef FRUITLIB_AUDIO_GROUP_SOUND_POSITIONAL_HPP_INCLUDED
#define FRUITLIB_AUDIO_GROUP_SOUND_POSITIONAL_HPP_INCLUDED

#include <fruitlib/audio/group/buffer_fwd.hpp>
#include <fruitlib/audio/group/player_fwd.hpp>
#include <fruitlib/audio/group/sound_base.hpp>
#include <sge/audio/sound/positional.hpp>
#include <sge/audio/sound/positional_parameters.hpp>
#include <sge/audio/sound/positional_unique_ptr.hpp>
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
	explicit
	sound_positional(
		group::buffer &,
		sge::audio::sound::positional_parameters const &,
		sge::audio::scalar gain,
		sge::audio::scalar pitch);

	explicit
	sound_positional(
		group::player &,
		sge::audio::sound::positional_unique_ptr,
		sge::audio::scalar gain,
		sge::audio::scalar pitch);

	void
	play(
		sge::audio::sound::repeat::type);

	void
	toggle_pause();

	sge::audio::sound::play_status::type
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
	position(
		sge::audio::vector const &);

	void
	linear_velocity(
		sge::audio::vector const &);

	void
	rolloff_factor(
		sge::audio::scalar);

	void
	reference_distance(
		sge::audio::scalar);

	void
	max_distance(
		sge::audio::scalar);

	void
	direction(
		sge::audio::sound::optional_direction const &);

	void
	inner_cone_angle(
		sge::audio::scalar);

	void
	outer_cone_angle(
		sge::audio::scalar);

	void
	outer_cone_gain(
		sge::audio::scalar);
private:
	sge::audio::sound::positional &impl_;
};
}
}
}

#endif
