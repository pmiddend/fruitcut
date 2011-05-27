#ifndef FRUITCUT_AUDIO_MUSIC_CONTROLLER_HPP_INCLUDED
#define FRUITCUT_AUDIO_MUSIC_CONTROLLER_HPP_INCLUDED

#include "../uniform_random.hpp"
#include <sge/audio/multi_loader_fwd.hpp>
#include <sge/audio/player_fwd.hpp>
#include <sge/audio/file_ptr.hpp>
#include <sge/audio/buffer_ptr.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/audio/sound/base_ptr.hpp>
#include <sge/time/timer.hpp>
#include <fcppt/string.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/filesystem/path.hpp>
#include <vector>
#include <map>

namespace fruitcut
{
namespace audio
{
class music_controller
{
FCPPT_NONCOPYABLE(
	music_controller);
public:
	explicit
	music_controller(
		sge::audio::multi_loader &,
		sge::audio::player &,
		sge::time::duration const &,
		fcppt::filesystem::path const &,
		sge::audio::scalar volume);

	void
	update();

	void
	play_event(
		fcppt::string const &);

	void
	play_random();

	void
	stop();

	~music_controller();
private:
	typedef
	std::map
	<
		fcppt::string,
		sge::audio::file_ptr
	>
	file_map;

	typedef
	std::vector<sge::audio::file_ptr>
	file_set;

	sge::audio::scalar volume_;
	file_map event_sounds_;
	file_set random_sounds_;
	sge::time::timer crossfade_;

	sge::audio::player &player_;
	sge::audio::buffer_ptr silence_buffer_;
	sge::audio::sound::base_ptr silence_source_;

	uniform_random<file_set::iterator::difference_type>::type random_element_rng_;

	sge::audio::sound::base_ptr 
		current_source_,
		new_source_;

	void
	do_play(
		sge::audio::sound::base_ptr);
};
}
}

#endif
