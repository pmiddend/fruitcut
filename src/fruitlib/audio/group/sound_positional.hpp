#ifndef FRUITLIB_AUDIO_GROUP_SOUND_POSITIONAL_HPP_INCLUDED
#define FRUITLIB_AUDIO_GROUP_SOUND_POSITIONAL_HPP_INCLUDED

#include <fruitlib/audio/group/sound_base.hpp>
#include <fruitlib/audio/group/buffer_fwd.hpp>
#include <fruitlib/audio/group/player_fwd.hpp>
#include <sge/audio/audio.hpp>
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
		sge::audio::sound::positional_ptr,
		sge::audio::scalar gain,
		sge::audio::scalar pitch);

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

	sge::audio::vector const 
	position() const;

	void 
	position(
		sge::audio::vector const &);

	sge::audio::vector const 
	linear_velocity() const;

	void 
	linear_velocity(
		sge::audio::vector const &);

	void 
	rolloff(
		sge::audio::scalar);

	sge::audio::scalar 
	rolloff() const;

	void 
	direction(
		sge::audio::vector const &);

	sge::audio::vector const 
	direction() const;

	void 
	inner_cone_angle(
		sge::audio::scalar);

	sge::audio::scalar 
	inner_cone_angle() const;

	void 
	outer_cone_angle(
		sge::audio::scalar);

	sge::audio::scalar 
	outer_cone_angle() const;
private:
	sge::audio::sound::positional &impl_;
};
}
}
}

#endif
