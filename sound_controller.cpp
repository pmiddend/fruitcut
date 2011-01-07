#include "sound_controller.hpp"
#include "media_path.hpp"
#include <sge/parse/json/find_member_exn.hpp>
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
#include <boost/spirit/home/phoenix/core.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/foreach.hpp>

fruitcut::sound_controller::sound_controller(
	sge::parse::json::object const &o,
	sge::audio::multi_loader &ml,
	sge::audio::player_ptr const _player)
:
	player_(
		_player),
	sounds_(),
	pool_()
{
	BOOST_FOREACH(
		sge::parse::json::member const &v,
		sge::parse::json::find_member_exn<sge::parse::json::object>(
			o.members,
			FCPPT_TEXT("events")).members)
		sounds_.insert(
			audio_map::value_type(
				v.name,
				player_->create_buffer(
					ml.load(
						media_path() 
							/ FCPPT_TEXT("sounds")
							/
								sge::parse::json::get<sge::parse::json::string>(
									v.value)))));
}

void
fruitcut::sound_controller::play(
	fcppt::string const &p)
{
	if (sounds_.find(p) == sounds_.end())
		throw sge::exception(FCPPT_TEXT("Couldn't find sound effect \"")+p+FCPPT_TEXT("\""));

	sge::audio::sound::base_ptr const b = 
		sounds_[p]->create_nonpositional();

	b->play(
		sge::audio::sound::repeat::once);

	pool_.add(
		b,
		sge::audio::stop_mode::play_once);
}

void
fruitcut::sound_controller::play_positional(
	fcppt::string const &p,
	sge::audio::sound::positional_parameters const &pp)
{
	if (sounds_.find(p) == sounds_.end())
		throw sge::exception(FCPPT_TEXT("Couldn't find sound effect \"")+p+FCPPT_TEXT("\""));

	sge::audio::sound::base_ptr const b = 
		sounds_[p]->create_positional(
			pp);

	b->play(
		sge::audio::sound::repeat::once);

	pool_.add(
		b,
		sge::audio::stop_mode::play_once);
}

void
fruitcut::sound_controller::update()
{
	pool_.update();
}

fruitcut::sound_controller::~sound_controller() {}
