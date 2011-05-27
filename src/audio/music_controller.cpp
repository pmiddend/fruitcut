#include "music_controller.hpp"
#include "../media_path.hpp"
#include "../create_rng.hpp"
#include "../exception.hpp"
#include "../resource_tree/from_directory_tree.hpp"
#include "../resource_tree/navigate_to_path.hpp"
#include "../resource_tree/path.hpp"
#include "../resource_tree/leaf_value.hpp"
#include <sge/audio/sound/repeat.hpp>
#include <sge/audio/sound/base.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/audio/multi_loader.hpp>
#include <sge/audio/buffer.hpp>
#include <fcppt/text.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/filesystem/directory_iterator.hpp>
#include <fcppt/filesystem/stem.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/filesystem/exists.hpp>
#include <fcppt/random/make_last_exclusive_range.hpp>
#include <boost/next_prior.hpp>
#include <iterator>

fruitcut::audio::music_controller::music_controller(
	sge::audio::multi_loader &ml,
	sge::audio::player &_player,
	sge::time::duration const &_crossfade,
	fcppt::filesystem::path const &_base_path,
	sge::audio::scalar const _volume)
:
	volume_(
		_volume),
	sounds_(
		fruitcut::resource_tree::from_directory_tree<sge::audio::file_ptr>(
			_base_path,
			std::tr1::bind(
				static_cast<sge::audio::file_ptr const (sge::audio::multi_loader::*)(fcppt::filesystem::path const &)>(
					&sge::audio::multi_loader::load),
				&ml,
				std::tr1::placeholders::_1))),
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
	random_element_generator_(
		resource_tree::navigate_to_path<sge::audio::file_ptr>(
			*sounds_,
			resource_tree::path(
				FCPPT_TEXT("random"))),
		fruitcut::create_rng()),
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
	do_play(
		player_.create_nonpositional_stream(
			resource_tree::leaf_value<sge::audio::file_ptr>(
				resource_tree::navigate_to_path<sge::audio::file_ptr>(
					*sounds_,
					resource_tree::path(FCPPT_TEXT("events"))/e))));
}

void
fruitcut::audio::music_controller::play_random()
{
	do_play(
		player_.create_nonpositional_stream(
			random_element_generator_.choose_random()));
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
