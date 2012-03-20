#ifndef FRUITLIB_AUDIO_MUSIC_CONTROLLER_HPP_INCLUDED
#define FRUITLIB_AUDIO_MUSIC_CONTROLLER_HPP_INCLUDED

#include <fruitlib/random_generator_fwd.hpp>
#include <fruitlib/uniform_int_random.hpp>
#include <fruitlib/audio/group/player.hpp>
#include <fruitlib/resource_tree/make_type.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/delta/callback.hpp>
#include <fruitlib/scenic/delta/clock.hpp>
#include <fruitlib/scenic/delta/duration.hpp>
#include <fruitlib/scenic/delta/timer.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/audio/buffer_ptr.hpp>
#include <sge/audio/file_ptr.hpp>
#include <sge/audio/loader_fwd.hpp>
#include <sge/audio/player_fwd.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/audio/sound/base_ptr.hpp>
#include <sge/timer/basic.hpp>
#include <sge/timer/clocks/delta.hpp>
#include <sge/timer/clocks/standard.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/shared_ptr.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <cstddef>
#include <map>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace audio
{
class music_controller
:
	public scenic::node<music_controller>
{
FCPPT_NONCOPYABLE(
	music_controller);
public:
	typedef
	boost::mpl::vector1<scenic::events::update>
	scene_reactions;

	explicit
	music_controller(
		scenic::optional_parent const &,
		scenic::delta::callback const &,
		fruitlib::random_generator &,
		sge::audio::loader &,
		sge::audio::player &,
		scenic::delta::duration const &,
		boost::filesystem::path const &,
		sge::audio::scalar volume);

	void
	play(
		resource_tree::path const &);

	void
	stop();

	sge::audio::scalar
	gain() const;

	void
	gain(
		sge::audio::scalar);

	void
	react(
		scenic::events::update const &);

	~music_controller();
private:
	typedef
	resource_tree::make_type
	<
		sge::audio::file_ptr,
		// shared_ptr because of horrible unique_ptr semantics
		fcppt::shared_ptr
		<
			fruitlib::uniform_int_random<std::size_t>::type
		>
	>::type
	resource_tree_type;

	typedef
	fcppt::unique_ptr<resource_tree_type>
	resource_tree_ptr;

	group::player player_;
	resource_tree_ptr sounds_;
	scenic::delta::clock clock_;
	scenic::delta::timer crossfade_;
	sge::audio::buffer_ptr silence_buffer_;
	sge::audio::sound::base_ptr silence_source_;

	sge::audio::sound::base_ptr
		current_source_,
		new_source_;

	void
	do_play(
		sge::audio::sound::base_ptr);

	sge::audio::file_ptr const
	choose_random(
		resource_tree_type const &);
};
}
}

#endif
