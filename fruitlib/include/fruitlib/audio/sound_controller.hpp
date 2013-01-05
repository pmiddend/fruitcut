#ifndef FRUITLIB_AUDIO_SOUND_CONTROLLER_HPP_INCLUDED
#define FRUITLIB_AUDIO_SOUND_CONTROLLER_HPP_INCLUDED

#include <fruitlib/random_generator_fwd.hpp>
#include <fruitlib/uniform_int_random.hpp>
#include <fruitlib/audio/pool.hpp>
#include <fruitlib/audio/group/player.hpp>
#include <fruitlib/resource_tree/make_type.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/audio/buffer_shared_ptr.hpp>
#include <sge/audio/loader_fwd.hpp>
#include <sge/audio/player_fwd.hpp>
#include <sge/audio/sound/base_unique_ptr.hpp>
#include <sge/audio/sound/positional_parameters.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <cstddef>
#include <memory>
#include <fcppt/config/external_end.hpp>


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
:
	public scenic::node<sound_controller>
{
FCPPT_NONCOPYABLE(
	sound_controller);
public:
	typedef
	boost::mpl::vector1<scenic::events::update>
	scene_reactions;

	explicit
	sound_controller(
		scenic::optional_parent const &,
		fruitlib::random_generator &,
		boost::filesystem::path const &,
		sge::audio::loader &,
		sge::audio::player &,
		sge::audio::scalar initial_gain);

	sge::audio::sound::base_unique_ptr
	create(
		fruitlib::resource_tree::path const &);

	void
	play(
		fruitlib::resource_tree::path const &);

	void
	play_positional(
		fruitlib::resource_tree::path const &,
		sge::audio::sound::positional_parameters const &);

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

	// The pool has update which needs to be called so the sounds are
	// deleted properly
	void
	react(
		scenic::events::update const &);

	~sound_controller();
private:
	typedef
	resource_tree::make_type
	<
		sge::audio::buffer_shared_ptr,
		// shared_ptr because of horrible unique_ptr semantics
		fcppt::shared_ptr
		<
			fruitlib::uniform_int_random<std::size_t>::type
		>
	>::type
	resource_tree_type;

	typedef
	std::unique_ptr<resource_tree_type>
	resource_tree_ptr;

	group::player player_;
	resource_tree_ptr sounds_;
	audio::pool pool_;

	void
	do_play(
		sge::audio::sound::base_unique_ptr);
};
}
}

#endif
