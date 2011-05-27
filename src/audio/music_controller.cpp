#include "music_controller.hpp"
#include "../media_path.hpp"
#include "../create_rng.hpp"
#include "../exception.hpp"
#include <sge/audio/sound/repeat.hpp>
#include <sge/audio/sound/base.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/audio/multi_loader.hpp>
#include <sge/audio/buffer.hpp>
#include <fcppt/text.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/filesystem/directory_iterator.hpp>
#include <fcppt/filesystem/stem.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/filesystem/exists.hpp>
#include <fcppt/random/make_last_exclusive_range.hpp>
#include <boost/next_prior.hpp>

namespace
{
template<typename Result>
Result const
parse_event_sounds(
	sge::audio::multi_loader &file_loader,
	fcppt::filesystem::path const &event_path)
{
	Result result;
	if(!fcppt::filesystem::exists(event_path))
		return result;
	for(
		fcppt::filesystem::directory_iterator current_sound(
			event_path); 
		current_sound != fcppt::filesystem::directory_iterator(); 
		++current_sound)
	{
		result.insert(
			result.end(),
			typename Result::value_type(
				fcppt::filesystem::stem(
					*current_sound),
				file_loader.load(
					*current_sound)));
	}
	return result;
}

template<typename Result>
Result const
parse_random_sounds(
	sge::audio::multi_loader &file_loader,
	fcppt::filesystem::path const &p)
{
	Result result;
	if(!fcppt::filesystem::exists(p))
		return result;
	for(
		fcppt::filesystem::directory_iterator current_sound(
			p); 
		current_sound != fcppt::filesystem::directory_iterator(); 
		++current_sound)
	{
		result.insert(
			result.end(),
			file_loader.load(
				*current_sound));
	}
	return result;
}
}

fruitcut::audio::music_controller::music_controller(
	sge::audio::multi_loader &ml,
	sge::audio::player &_player,
	sge::time::duration const &_crossfade,
	fcppt::filesystem::path const &_base_path,
	sge::audio::scalar const _volume)
:
	volume_(
		_volume),
	event_sounds_(
		parse_event_sounds<file_map>(
			ml,
			_base_path / FCPPT_TEXT("events"))),
	random_sounds_(
		parse_random_sounds<file_set>(
			ml,
			_base_path / FCPPT_TEXT("random"))),
	crossfade_(
		_crossfade),
	player_(
		_player),
	silence_buffer_(
		player_.create_buffer(
			*ml.load(
				_base_path/FCPPT_TEXT("silence.wav")))),
	silence_source_(
		silence_buffer_->create_nonpositional()),
	random_element_rng_(
		fcppt::random::make_last_exclusive_range<file_set::iterator::difference_type>(
			static_cast<file_set::iterator::difference_type>(
				0),
			static_cast<file_set::iterator::difference_type>(
				random_sounds_.size())),
		create_rng()),
	current_source_(
		silence_source_),
	new_source_()
{
}

void
fruitcut::audio::music_controller::update()
{
	if (new_source_)
	{
		if (crossfade_.expired())
		{
			FCPPT_ASSERT(
				current_source_);
			current_source_->stop();
			current_source_ = new_source_;
			new_source_.reset();
		}
		else
		{
			current_source_->gain(
				//static_cast<sge::audio::scalar>(1) - 
				volume_ 
					- static_cast<sge::audio::scalar>(
						crossfade_.elapsed_frames()) 
					* volume_);
			new_source_->gain(
				static_cast<sge::audio::scalar>(
					crossfade_.elapsed_frames())
					* volume_);

			new_source_->update();
		}
	}

	if (current_source_)
		current_source_->update();
}

void
fruitcut::audio::music_controller::play_event(
	fcppt::string const &e)
{
	if (event_sounds_.find(e) == event_sounds_.end())
		throw exception(FCPPT_TEXT("Event \"")+e+FCPPT_TEXT("\" not found"));

	do_play(
		player_.create_nonpositional_stream(
			event_sounds_[e]));
}

void
fruitcut::audio::music_controller::play_random()
{
	if (random_sounds_.empty())
		return;
	
	do_play(
		player_.create_nonpositional_stream(
			*boost::next(
				random_sounds_.begin(),
				random_element_rng_())));
}

void
fruitcut::audio::music_controller::stop()
{
	do_play(
		silence_source_);
}

fruitcut::audio::music_controller::~music_controller() {}

void
fruitcut::audio::music_controller::do_play(
	sge::audio::sound::base_ptr b)
{
	b->gain(
		static_cast<sge::audio::scalar>(0));
	b->play(
		sge::audio::sound::repeat::once);
	crossfade_.reset();

	new_source_ = b;
}
