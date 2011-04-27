#include "music_controller.hpp"
#include "../string_to_duration_exn.hpp"
#include "../json/find_member.hpp"
#include "../json/convert.hpp"
#include "../media_path.hpp"
#include "../create_rng.hpp"
#include "../exception.hpp"
#include <sge/audio/sound/repeat.hpp>
#include <sge/audio/sound/base.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/audio/multi_loader.hpp>
#include <sge/audio/buffer.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <fcppt/text.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/random/make_last_exclusive_range.hpp>
#include <boost/next_prior.hpp>
#include <boost/spirit/home/phoenix/object.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/core.hpp>
#include <boost/spirit/home/phoenix/operator.hpp>

fruitcut::audio::music_controller::music_controller(
	sge::parse::json::object const &o,
	sge::audio::multi_loader &ml,
	sge::audio::player_ptr const _player)
:
	volume_(
		json::find_member<sge::audio::scalar>(
			o,
			FCPPT_TEXT("volume"))),
	event_sounds_(
		fcppt::algorithm::map<file_map>(
			json::find_member<sge::parse::json::object>(
				o,
				FCPPT_TEXT("events")).members,
			boost::phoenix::construct<file_map::value_type>(
				boost::phoenix::bind(
					&sge::parse::json::member::name,
					boost::phoenix::arg_names::arg1),
				boost::phoenix::bind(
					static_cast<sge::audio::file_ptr const (sge::audio::multi_loader::*)(fcppt::filesystem::path const &)>(
						&sge::audio::multi_loader::load),
					&ml,
					boost::phoenix::val(
						media_path() / FCPPT_TEXT("music")) / 
					boost::phoenix::bind(
						&json::convert<fcppt::string>,
						boost::phoenix::bind(
							&sge::parse::json::member::value,
							boost::phoenix::arg_names::arg1)))))),
	random_sounds_(
		fcppt::algorithm::map<file_set>(
			json::find_member<sge::parse::json::array>(
				o,
				FCPPT_TEXT("random")).elements,
			boost::phoenix::bind(
				static_cast<sge::audio::file_ptr const (sge::audio::multi_loader::*)(fcppt::filesystem::path const &)>(
					&sge::audio::multi_loader::load),
				&ml,
				boost::phoenix::val(
					media_path() / FCPPT_TEXT("music")) / 
				boost::phoenix::bind(
					&json::convert<fcppt::string>,
					boost::phoenix::arg_names::arg1)))),
	crossfade_(
		string_to_duration_exn<sge::time::duration>(
			json::find_member<fcppt::string>(
				o,
				FCPPT_TEXT("crossfade-time")))),
	player_(
		_player),
	silence_buffer_(
		player_->create_buffer(
			ml.load(
				media_path()/FCPPT_TEXT("sounds")/FCPPT_TEXT("silence.wav")))),
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
		player_->create_nonpositional_stream(
			event_sounds_[e]));
}

void
fruitcut::audio::music_controller::play_random()
{
	if (random_sounds_.empty())
		return;
	
	do_play(
		player_->create_nonpositional_stream(
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
