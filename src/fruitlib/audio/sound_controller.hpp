#ifndef FRUITCUT_FRUITLIB_AUDIO_SOUND_CONTROLLER_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_AUDIO_SOUND_CONTROLLER_HPP_INCLUDED

#include "../resource_tree/make_type.hpp"
#include "../resource_tree/path.hpp"
#include "../uniform_random.hpp"
#include "../rng_creator_fwd.hpp"
#include "group/player.hpp"
#include <sge/audio/multi_loader_fwd.hpp>
#include <sge/audio/player_fwd.hpp>
#include <sge/audio/pool/object.hpp>
#include <sge/audio/sound/positional_parameters.hpp>
#include <sge/audio/sound/base_ptr.hpp>
#include <sge/audio/buffer_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/filesystem/path.hpp>
#include <cstddef>

namespace fruitcut
{
namespace fruitlib
{
namespace audio
{
/// The sound_controller is very similar to the 
/// music_controller. Sounds shouldn't be hard-coded, so we store a
/// map
///
/// sound_name -> filename 
/// 
/// In a json file and convert it to a map
///
/// sound_name -> sound_group 
/// 
/// In this controller. We use sound buffers, so the sounds are
/// non-streaming. This class also includes the sound pool, which
/// solves the problem that a sound stops playing when the destructor is
/// called (we want the sound to be destroyed when the destructor is
/// called _and_ it's finished playing. Use case: a character dies).
///
/// Note that the sound controller wasn't designed for engine sounds
/// since those are defined per vehicle; also they're looping and
/// should end when the vehicle dies, so putting them in the pool
/// isn't really appropriate
/// 
/// Also, we store "sound groups", which aggregate more than one sound
/// so you can randomly play one of them.
class sound_controller
{
FCPPT_NONCOPYABLE(
	sound_controller);
public:
	explicit 
	sound_controller(
		fruitlib::rng_creator &,
		fcppt::filesystem::path const &,
		sge::audio::multi_loader &,
		sge::audio::player &,
		sge::audio::scalar initial_gain);

	void
	play(
		resource_tree::path const &);

	void
	play_positional(
		resource_tree::path const &,
		sge::audio::sound::positional_parameters const &);

	// The pool has update which needs to be called so the sounds are
	// deleted properly
	void
	update();

	sge::audio::scalar
	gain() const;

	void
	gain(
		sge::audio::scalar);

	sge::audio::scalar
	pitch() const;

	void
	pitch(
		sge::audio::scalar);

	~sound_controller();
private:
	typedef
	resource_tree::make_type
	<
		sge::audio::buffer_ptr,
		fruitlib::uniform_random
		<
			std::size_t
		>::type
	>::type
	resource_tree_type;

	typedef
	fcppt::unique_ptr<resource_tree_type>
	resource_tree_ptr;

	group::player player_;
	resource_tree_ptr sounds_;
	sge::audio::pool::object pool_;

	void
	do_play(
		sge::audio::sound::base_ptr);
};
}
}
}

#endif
