#include "music_controller.hpp"
#include "../media_path.hpp"
#include "../create_rng.hpp"
#include "../exception.hpp"
#include "../resource_tree/from_directory_tree.hpp"
#include "../resource_tree/navigate_to_path.hpp"
#include "../resource_tree/path.hpp"
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
#include <fcppt/filesystem/path.hpp>
#include <fcppt/random/make_last_exclusive_range.hpp>
#include <fcppt/filesystem/directory_iterator.hpp>
#include <boost/next_prior.hpp>
#include <iterator>

namespace
{
fruitcut::uniform_random<std::size_t>::type
create_random_from_directory(
	fcppt::filesystem::path const &p)
{
	return 
		fruitcut::uniform_random<std::size_t>::type(
			fcppt::random::make_last_exclusive_range(
				static_cast<std::size_t>(
					0),
				static_cast<std::size_t>(
					std::distance(
						fcppt::filesystem::directory_iterator(
							p),
						fcppt::filesystem::directory_iterator()))),
				fruitcut::create_rng());
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
	sounds_(
		fruitcut::resource_tree::from_directory_tree
		<
			sge::audio::file_ptr,
			fruitcut::uniform_random
			<
				std::size_t
			>::type
		>(
			_base_path,
			std::tr1::bind(
				static_cast<sge::audio::file_ptr const (sge::audio::multi_loader::*)(fcppt::filesystem::path const &)>(
					&sge::audio::multi_loader::load),
				&ml,
				std::tr1::placeholders::_1),
			&create_random_from_directory)),
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
fruitcut::audio::music_controller::play(
	resource_tree::path const &target_path)
{
	resource_tree_type &target_tree =
		resource_tree::navigate_to_path(
			*sounds_,
			target_path);

	if(target_tree.value().is_leaf())
	{
		do_play(
			player_.create_nonpositional_stream(
				target_tree.value().leaf_value()));
	}
	else
	{
		resource_tree_type &target_file = 
			*boost::next(
				target_tree.begin(),
				static_cast<std::iterator_traits<resource_tree_type::const_iterator>::difference_type>(
					target_tree.value().node_value()()));

		if(!target_file.value().is_leaf())
			throw fruitcut::exception(FCPPT_TEXT("The argument to play() must be either a file or a directory containing just files!\nThat was not the case for: ")+target_path.string());

		do_play(
			player_.create_nonpositional_stream(
				target_file.value().leaf_value()));
	}
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
