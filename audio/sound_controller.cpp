#include "sound_controller.hpp"
#include "detail/buffer_sequence.hpp"
#include "detail/sound_group.hpp"
#include "../media_path.hpp"
#include "../json/array_to_vector.hpp"
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/member_vector.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/string.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/member.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/audio/buffer.hpp>
#include <sge/audio/sound/repeat.hpp>
#include <sge/audio/multi_loader.hpp>
#include <sge/audio/stop_mode.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/sound/positional.hpp>
#include <sge/exception.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/string.hpp>
#include <boost/spirit/home/phoenix/core.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/foreach.hpp>
#include <iostream>

fruitcut::audio::sound_controller::sound_controller(
	sge::parse::json::object const &sound_array,
	sge::audio::multi_loader &ml,
	sge::audio::player_ptr const _player)
:
	player_(
		_player),
	sounds_(),
	pool_()
{
	BOOST_FOREACH(
		sge::parse::json::member_vector::const_reference current_sound,
		sound_array.members)
	{
		// This is an algorithm::map call, but I don't want to use phoenix
		// here, too convoluted
		detail::buffer_sequence buffers;
		BOOST_FOREACH(
			fcppt::string const &current_file,
			json::array_to_vector<fcppt::string>(
				sge::parse::json::get<sge::parse::json::array>(
					current_sound.value)))
		{
			std::cerr << "processing file " << current_file << "\n";
			buffers.push_back(
				player_->create_buffer(
					ml.load(
						media_path() 
							/ FCPPT_TEXT("sounds")
							/
								current_file)));
		}
		FCPPT_ASSERT_MESSAGE(
			!buffers.empty(),
			FCPPT_TEXT("Got an empty sound group!"));
		sounds_.insert(
			std::make_pair(
				current_sound.name,
				buffers));
	}
}

void
fruitcut::audio::sound_controller::play(
	fcppt::string const &p)
{
	audio_map::iterator const current_sound = 
		sounds_.find(
			p);

	if (current_sound == sounds_.end())
		throw sge::exception(FCPPT_TEXT("Couldn't find sound effect \"")+p+FCPPT_TEXT("\""));

	sge::audio::sound::base_ptr const b = 
		current_sound->second.random_element()->create_nonpositional();

	b->play(
		sge::audio::sound::repeat::once);

	pool_.add(
		b,
		sge::audio::stop_mode::play_once);
}

void
fruitcut::audio::sound_controller::play_positional(
	fcppt::string const &p,
	sge::audio::sound::positional_parameters const &pp)
{
	audio_map::iterator const current_sound = 
		sounds_.find(
			p);

	if (current_sound == sounds_.end())
		throw sge::exception(FCPPT_TEXT("Couldn't find sound effect \"")+p+FCPPT_TEXT("\""));

	sge::audio::sound::base_ptr const b = 
		current_sound->second.random_element()->create_positional(
			pp);

	b->play(
		sge::audio::sound::repeat::once);

	pool_.add(
		b,
		sge::audio::stop_mode::play_once);
}

void
fruitcut::audio::sound_controller::update()
{
	pool_.update();
}

fruitcut::audio::sound_controller::~sound_controller() {}
