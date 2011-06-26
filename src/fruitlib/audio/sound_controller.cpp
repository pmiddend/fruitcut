#include "sound_controller.hpp"
#include "../resource_tree/navigate_to_path.hpp"
#include "../resource_tree/from_directory_tree.hpp"
#include "../resource_tree/path.hpp"
#include "../rng_creator.hpp"
#include "../uniform_random.hpp"
#include "../exception.hpp"
#include <sge/audio/buffer.hpp>
#include <sge/audio/buffer_ptr.hpp>
#include <sge/audio/multi_loader.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/sound/positional.hpp>
#include <sge/audio/sound/repeat.hpp>
#include <sge/audio/pool/stop_mode.hpp>
#include <fcppt/filesystem/directory_iterator.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/random/make_last_exclusive_range.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <iostream>
#include <iterator>
#include <cstddef>

namespace
{
fruitcut::fruitlib::uniform_random<std::size_t>::type
create_random_from_directory(
	fruitcut::fruitlib::rng_creator &_rng_creator,
	fcppt::filesystem::path const &p)
{
	return 
		fruitcut::fruitlib::uniform_random<std::size_t>::type(
			fcppt::random::make_last_exclusive_range(
				static_cast<std::size_t>(
					0),
				static_cast<std::size_t>(
					std::distance(
						fcppt::filesystem::directory_iterator(
							p),
						fcppt::filesystem::directory_iterator()))),
				_rng_creator.create());
}

sge::audio::buffer_ptr const
create_buffer_from_path(
	sge::audio::multi_loader &loader,
	sge::audio::player &player,
	fcppt::filesystem::path const &file)
{
	return 
		player.create_buffer(
			*loader.load(
				file));
}
}

fruitcut::fruitlib::audio::sound_controller::sound_controller(
	fruitlib::rng_creator &_rng_creator,
	fcppt::filesystem::path const &_base_path,
	sge::audio::multi_loader &_loader,
	sge::audio::player &_player,
	sge::audio::pool::gain_factor const &_gain_factor)
:
	player_(
		_player),
	sounds_(
		fruitlib::resource_tree::from_directory_tree<resource_tree_type>(
			_base_path,
			std::tr1::bind(
				&create_buffer_from_path,
				fcppt::ref(
					_loader),
				fcppt::ref(
					_player),
				std::tr1::placeholders::_1),
			std::tr1::bind(
				&create_random_from_directory,
				fcppt::ref(
					_rng_creator),
				std::tr1::placeholders::_1))),
	pool_(
		_gain_factor,
		sge::audio::pool::pitch_factor(
			static_cast<sge::audio::pool::pitch_factor::value_type>(
				1)))
{
}

void
fruitcut::fruitlib::audio::sound_controller::play(
	resource_tree::path const &target_path)
{
	resource_tree_type &target_tree =
		resource_tree::navigate_to_path(
			*sounds_,
			target_path);

	if(target_tree.value().is_leaf())
	{
		do_play(
			target_tree.value().leaf_value()->create_nonpositional());
	}
	else
	{
		resource_tree_type &target_file = 
			*boost::next(
				target_tree.begin(),
				static_cast<std::iterator_traits<resource_tree_type::const_iterator>::difference_type>(
					target_tree.value().node_value()()));

		if(!target_file.value().is_leaf())
			throw fruitlib::exception(FCPPT_TEXT("The argument to play() must be either a file or a directory containing just files!\nThat was not the case for: ")+target_path.string());

		do_play(
			target_file.value().leaf_value()->create_nonpositional());
	}
}

void
fruitcut::fruitlib::audio::sound_controller::play_positional(
	resource_tree::path const &target_path,
	sge::audio::sound::positional_parameters const &pp)
{
	resource_tree_type &target_tree =
		resource_tree::navigate_to_path(
			*sounds_,
			target_path);

	if(target_tree.value().is_leaf())
	{
		do_play(
			target_tree.value().leaf_value()->create_positional(
				pp));
	}
	else
	{
		resource_tree_type &target_file = 
			*boost::next(
				target_tree.begin(),
				static_cast<std::iterator_traits<resource_tree_type::const_iterator>::difference_type>(
					target_tree.value().node_value()()));

		if(!target_file.value().is_leaf())
			throw fruitlib::exception(FCPPT_TEXT("The argument to play() must be either a file or a directory containing just files!\nThat was not the case for: ")+target_path.string());

		do_play(
			target_file.value().leaf_value()->create_positional(
				pp));
	}
}

void
fruitcut::fruitlib::audio::sound_controller::update()
{
	pool_.update();
}

sge::audio::pool::gain_factor::value_type
fruitcut::fruitlib::audio::sound_controller::gain_factor() const
{
	return 
		pool_.gain_factor();
}

void
fruitcut::fruitlib::audio::sound_controller::gain_factor(
	sge::audio::pool::gain_factor::value_type const _gain)
{
	pool_.gain_factor(
		_gain);
}

sge::audio::pool::pitch_factor::value_type
fruitcut::fruitlib::audio::sound_controller::pitch_factor() const
{
	return 
		pool_.pitch_factor();
}

void
fruitcut::fruitlib::audio::sound_controller::pitch_factor(
	sge::audio::pool::pitch_factor::value_type const _pitch)
{
	pool_.pitch_factor(
		_pitch);
}

fruitcut::fruitlib::audio::sound_controller::~sound_controller() {}

void
fruitcut::fruitlib::audio::sound_controller::do_play(
	sge::audio::sound::base_ptr const b)
{
	b->play(
		sge::audio::sound::repeat::once);

	pool_.add(
		b,
		sge::audio::pool::stop_mode::play_once);
}
