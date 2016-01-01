#ifndef FRUITLIB_AUDIO_MUSIC_CONTROLLER_HPP_INCLUDED
#define FRUITLIB_AUDIO_MUSIC_CONTROLLER_HPP_INCLUDED

#include <fruitlib/random_generator_fwd.hpp>
#include <fruitlib/uniform_int_random.hpp>
#include <fruitlib/audio/group/player.hpp>
#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/resource_tree/make_type.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/delta/callback.hpp>
#include <fruitlib/scenic/delta/clock.hpp>
#include <fruitlib/scenic/delta/duration.hpp>
#include <fruitlib/scenic/delta/timer.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/audio/buffer_unique_ptr.hpp>
#include <sge/audio/file_fwd.hpp>
#include <sge/audio/loader_fwd.hpp>
#include <sge/audio/player_fwd.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/audio/sound/base_shared_ptr.hpp>
#include <sge/timer/basic.hpp>
#include <sge/timer/clocks/delta.hpp>
#include <sge/timer/clocks/standard.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/shared_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <cstddef>
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

	FRUITLIB_DETAIL_SYMBOL
	music_controller(
		scenic::optional_parent const &,
		scenic::delta::callback const &,
		fruitlib::random_generator &,
		sge::audio::loader &,
		sge::audio::player &,
		scenic::delta::duration const &,
		boost::filesystem::path const &,
		sge::audio::scalar volume);

	FRUITLIB_DETAIL_SYMBOL
	void
	play(
		resource_tree::path const &);

	void
	stop();

	sge::audio::scalar
	gain() const;

	FRUITLIB_DETAIL_SYMBOL
	void
	gain(
		sge::audio::scalar);

	void
	react(
		scenic::events::update const &);

	FRUITLIB_DETAIL_SYMBOL
	~music_controller();
private:
	typedef
	fcppt::shared_ptr<
		sge::audio::file
	>
	file_shared_ptr;

	typedef
	resource_tree::make_type
	<
		file_shared_ptr,
		// shared_ptr because of horrible unique_ptr semantics
		fcppt::shared_ptr
		<
			fruitlib::uniform_int_random<std::size_t>::type
		>
	>::type
	resource_tree_type;

	group::player player_;
	resource_tree_type sounds_;
	scenic::delta::clock clock_;
	scenic::delta::timer crossfade_;
	sge::audio::buffer_unique_ptr const silence_buffer_;
	sge::audio::sound::base_shared_ptr silence_source_;

	sge::audio::sound::base_shared_ptr current_source_;

	typedef
	fcppt::optional::object<
		sge::audio::sound::base_shared_ptr
	>
	optional_sound_shared_ptr;

	optional_sound_shared_ptr new_source_;

	void
	do_play(
		sge::audio::sound::base_shared_ptr);
};
}
}

#endif
