#include "highscore.hpp"
#include "../intro.hpp"
#include "../../events/gameover/continue_button_pushed.hpp"
#include "../../../exception.hpp"
#include "../../../media_path.hpp"
#include <sge/cegui/to_cegui_string.hpp>
#include <sge/cegui/from_cegui_string.hpp>
#include <sge/cegui/toolbox/append_row.hpp>
#include <sge/cegui/toolbox/row.hpp>
#include <sge/config/find_cache_path.hpp>
#include <CEGUI/CEGUIEvent.h>
#include <CEGUI/CEGUIString.h>
#include <CEGUI/CEGUIWindow.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/elements/CEGUIPushButton.h>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/string.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/lexical_cast.hpp>
#include <boost/range/algorithm/sort.hpp>

#include "../../score.hpp"
#include "../../name.hpp"
#include "../../../json/find_member.hpp"
#include "../../../json/array_to_vector.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_file_exn.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/value.hpp>
#include <fcppt/filesystem/exists.hpp>
#include <fcppt/filesystem/path_to_string.hpp>
#include <fcppt/filesystem/remove_filename.hpp>
#include <fcppt/filesystem/create_directories_recursive.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/io/ofstream.hpp>
#include <fcppt/from_std_string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/foreach.hpp>

namespace
{
struct highscore_entry
{
public:
	fcppt::string name;
	fruitcut::app::score score_;
	boost::posix_time::ptime date_time;

	explicit
	highscore_entry(
		fcppt::string const &_name,
		fruitcut::app::score const _score,
		boost::posix_time::ptime _date_time)
	:
		name(
			_name),
		score_(
			_score),
		date_time(
			_date_time)
	{
	}

	bool 
	operator<(
		highscore_entry const &right) const
	{
		return 
			score_ == right.score_
			?
				date_time < right.date_time
			:
				score_ > right.score_; 
	}
};

typedef
std::vector<highscore_entry>
highscore_sequence;

highscore_sequence
load_highscore()
{
	fcppt::filesystem::path const highscore_file_path = 
		sge::config::find_cache_path(
			fruitcut::app::name())/
		FCPPT_TEXT("highscore.json");

	if(!fcppt::filesystem::exists(highscore_file_path))
		return highscore_sequence();

	sge::parse::json::array const json_file = 
		fruitcut::json::find_member<sge::parse::json::array>(
			sge::parse::json::parse_file_exn(
				highscore_file_path),
			FCPPT_TEXT("entries"));

	highscore_sequence result;

	BOOST_FOREACH(
		sge::parse::json::object const &current_entry,
		fruitcut::json::array_to_vector<sge::parse::json::object>(
			json_file))
	{
		result.push_back(
			highscore_entry(
				fruitcut::json::find_member<sge::parse::json::string>(
					current_entry,
					FCPPT_TEXT("name")),
				fruitcut::json::find_member<fruitcut::app::score>(
					current_entry,
					FCPPT_TEXT("score")),
				boost::posix_time::from_iso_string(
					fruitcut::json::find_member<sge::parse::json::string>(
						current_entry,
						FCPPT_TEXT("date-time")))));
	}

	return result;
}

void
write_highscore(
	highscore_sequence const &entries)
{
	fcppt::filesystem::path const highscore_path = 
		sge::config::find_cache_path(
			fruitcut::app::name())/
		FCPPT_TEXT("highscore.json");

	if(!fcppt::filesystem::exists(highscore_path))
	{
		fcppt::filesystem::create_directories_recursive(
			fcppt::filesystem::remove_filename(
				highscore_path));
	}

	fcppt::io::ofstream stream(
		highscore_path);

	if(!stream.is_open())
		throw 
			fruitcut::exception(
				FCPPT_TEXT("Couldn't open highscore file \"")+
				fcppt::filesystem::path_to_string(
					highscore_path)+
				FCPPT_TEXT("\""));

	stream << FCPPT_TEXT("{\n\t\"entries\" : \n\t[\n");
	for(highscore_sequence::const_iterator hs = entries.begin(); hs != entries.end(); ++hs)
	{
		stream << "\t\t{\n";
		stream << "\t\t\t\"name\" : \"" << hs->name << "\",\n";
		stream << "\t\t\t\"score\" : " << hs->score_ << ",\n";
		stream << "\t\t\t\"date-time\" : \"" << fcppt::from_std_string(boost::posix_time::to_iso_string(hs->date_time)) << "\"\n";
		if (hs == --entries.end())
			stream << "\t\t}";
		else
			stream << "\t\t},\n";
	}
	stream << FCPPT_TEXT("\n\t]\n}\n");
}
}

fruitcut::app::states::gameover::highscore::highscore(
	my_context ctx)
:
	my_base(
		ctx),
	layout_(
		media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("layouts")/FCPPT_TEXT("highscore.layout"),
		context<machine>().systems().charconv_system()),
	gui_sheet_(
		*CEGUI::WindowManager::getSingleton().getWindow("Highscore")),
	quit_button_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("Highscore/QuitButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&highscore::quit_button_pushed,
					this,
					std::tr1::placeholders::_1)))),
	reset_button_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("Highscore/ResetButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&highscore::reset_button_pushed,
					this,
					std::tr1::placeholders::_1))))
{
	highscore_sequence entries = 
		load_highscore();

	entries.push_back(
		highscore_entry(
			sge::cegui::from_cegui_string(
				context<superstate>().name(),
				context<machine>().systems().charconv_system()),
			context<machine>().last_game_score(),
			boost::posix_time::second_clock::universal_time()));

	boost::sort(
		entries);

	write_highscore(
		entries);

	unsigned index = 1;
	BOOST_FOREACH(
		highscore_entry const &current_entry,
		entries)
	{
		typedef 
		boost::date_time::c_local_adjustor<boost::posix_time::ptime> 
		local_adjuster;

		sge::cegui::toolbox::append_row(
			dynamic_cast<CEGUI::MultiColumnList &>(
				*CEGUI::WindowManager::getSingleton().getWindow("Highscore/List")),
			fcppt::assign::make_container<sge::cegui::toolbox::row>
				(sge::cegui::from_cegui_string(
					fcppt::lexical_cast<fcppt::string>(
						index),
					context<machine>().systems().charconv_system()))
				(sge::cegui::from_cegui_string(
					current_entry.name,
					context<machine>().systems().charconv_system()))
				(sge::cegui::from_cegui_string(
					fcppt::lexical_cast<fcppt::string>(
						current_entry.score_),
					context<machine>().systems().charconv_system()))
				(sge::cegui::from_cegui_string(
					boost::posix_time::to_simple_string(
						local_adjuster::utc_to_local(
							current_entry.date_time)),
					context<machine>().systems().charconv_system())));
	}
}

boost::statechart::result
fruitcut::app::states::gameover::highscore::react(
	events::gameover::quit_button_pushed const &)
{
	context<machine>().quit();
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::gameover::highscore::react(
	events::gameover::reset_button_pushed const &)
{
	return transit<intro>();
}

fruitcut::app::states::gameover::highscore::~highscore()
{
}

bool
fruitcut::app::states::gameover::highscore::quit_button_pushed(
	CEGUI::EventArgs const &)
{
	context<machine>().sound_controller().play(
		FCPPT_TEXT("button-clicked"));
	post_event(
		events::gameover::quit_button_pushed());
	return true;
}

bool
fruitcut::app::states::gameover::highscore::reset_button_pushed(
	CEGUI::EventArgs const &)
{
	context<machine>().sound_controller().play(
		FCPPT_TEXT("button-clicked"));
	post_event(
		events::gameover::reset_button_pushed());
	return true;
}
