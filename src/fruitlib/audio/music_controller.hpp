#ifndef FRUITLIB_AUDIO_MUSIC_CONTROLLER_HPP_INCLUDED
#define FRUITLIB_AUDIO_MUSIC_CONTROLLER_HPP_INCLUDED

#include "../uniform_random.hpp"
#include "../random_generator.hpp"
#include "../resource_tree/make_type.hpp"
#include "../resource_tree/path.hpp"
#include "../scenic/node.hpp"
#include "../scenic/optional_parent.hpp"
#include "../scenic/events/update_fwd.hpp"
#include "group/player.hpp"
#include <sge/audio/multi_loader_fwd.hpp>
#include <sge/audio/player_fwd.hpp>
#include <sge/audio/file_ptr.hpp>
#include <sge/audio/buffer_ptr.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/audio/sound/base_ptr.hpp>
#include <sge/time/timer.hpp>
#include <fcppt/string.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/filesystem/path.hpp>
#include <cstddef>
#include <vector>
#include <map>

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
		fruitlib::random_generator &,
		sge::audio::multi_loader &,
		sge::audio::player &,
		sge::time::duration const &,
		fcppt::filesystem::path const &,
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
	sge::time::timer crossfade_;

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
