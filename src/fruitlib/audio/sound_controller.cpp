#include <fruitlib/exception.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/resource_tree/from_directory_tree.hpp>
#include <fruitlib/resource_tree/navigate_to_path.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <sge/audio/buffer.hpp>
#include <sge/audio/buffer_ptr.hpp>
#include <sge/audio/loader.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/sound/nonpositional_parameters.hpp>
#include <sge/audio/sound/positional.hpp>
#include <sge/audio/sound/repeat.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <fcppt/config/external_end.hpp>


namespace
{
fcppt::shared_ptr
<
	fruitlib::uniform_int_random<std::size_t>::type
>
create_random_from_directory(
	fruitlib::random_generator &_random_generator,
	boost::filesystem::path const &p)
{
	typedef
	fruitlib::uniform_int_random<std::size_t>::type
	random_variate;

	return
		fcppt::make_shared_ptr<random_variate>(
			fcppt::ref(
				_random_generator),
			random_variate::distribution(
				random_variate::distribution::min(
					static_cast<std::size_t>(
						0)),
				random_variate::distribution::max(
					static_cast<std::size_t>(
						std::distance(
							boost::filesystem::directory_iterator(
								p),
							boost::filesystem::directory_iterator()))-1)));
}

sge::audio::buffer_ptr const
create_buffer_from_path(
	sge::audio::loader &loader,
	sge::audio::player &player,
	boost::filesystem::path const &file)
{
	return
		player.create_buffer(
			*loader.load(
				file));
}
}

fruitlib::audio::sound_controller::sound_controller(
	scenic::optional_parent const &_parent,
	fruitlib::random_generator &_random_generator,
	boost::filesystem::path const &_base_path,
	sge::audio::loader &_loader,
	sge::audio::player &_player,
	sge::audio::scalar const _initial_gain)
:
	node_base(
		_parent),
	player_(
		_player,
		_initial_gain,
		// Pitch
		static_cast<sge::audio::scalar>(
			1)),
	sounds_(
		fruitlib::resource_tree::from_directory_tree<resource_tree_type>(
			_base_path,
			std::tr1::bind(
				&create_buffer_from_path,
				fcppt::ref(
					_loader),
				fcppt::ref(
					player_),
				std::tr1::placeholders::_1),
			std::tr1::bind(
				&create_random_from_directory,
				fcppt::ref(
					_random_generator),
				std::tr1::placeholders::_1))),
	pool_()
{
}

void
fruitlib::audio::sound_controller::play(
	resource_tree::path const &target_path)
{
	resource_tree_type &target_tree =
		resource_tree::navigate_to_path(
			*sounds_,
			target_path);

	if(target_tree.value().is_leaf())
	{
		do_play(
			target_tree.value().leaf_value()->create_nonpositional(
				sge::audio::sound::nonpositional_parameters()));
	}
	else
	{
		resource_tree_type &target_file =
			*boost::next(
				target_tree.begin(),
				static_cast<std::iterator_traits<resource_tree_type::const_iterator>::difference_type>(
					(*target_tree.value().node_value())()));

		if(!target_file.value().is_leaf())
			throw fruitlib::exception(FCPPT_TEXT("The argument to play() must be either a file or a directory containing just files!\nThat was not the case for: ")+target_path.string());

		do_play(
			target_file.value().leaf_value()->create_nonpositional(
				sge::audio::sound::nonpositional_parameters()));
	}
}

void
fruitlib::audio::sound_controller::play_positional(
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
					(*target_tree.value().node_value())()));

		if(!target_file.value().is_leaf())
			throw fruitlib::exception(FCPPT_TEXT("The argument to play() must be either a file or a directory containing just files!\nThat was not the case for: ")+target_path.string());

		do_play(
			target_file.value().leaf_value()->create_positional(
				pp));
	}
}

sge::audio::scalar
fruitlib::audio::sound_controller::gain() const
{
	return
		player_.gain();
}

void
fruitlib::audio::sound_controller::gain(
	sge::audio::scalar const _gain)
{
	player_.gain(
		_gain);
}

sge::audio::scalar
fruitlib::audio::sound_controller::pitch() const
{
	return
		player_.pitch();
}

void
fruitlib::audio::sound_controller::pitch(
	sge::audio::scalar const _pitch)
{
	player_.pitch(
		_pitch);
}

void
fruitlib::audio::sound_controller::react(
	scenic::events::update const &)
{
	pool_.update();
}

fruitlib::audio::sound_controller::~sound_controller() {}


void
fruitlib::audio::sound_controller::do_play(
	sge::audio::sound::base_ptr const b)
{
	b->play(
		sge::audio::sound::repeat::once);

	pool_.insert(
		b);
}
