#include "highscore.hpp"
#include "../menu/main.hpp"
#include "../ingame/running.hpp"
#include "../../exception.hpp"
#include "../../../media_path.hpp"
#include "../../../fruitlib/audio/sound_controller.hpp"
#include "../../../fruitlib/resource_tree/path.hpp"
#include "../../events/define_transition_reaction.hpp"
#include "../../events/return_post_transition_functor.hpp"
#include "../../events/post_transition.hpp"
#include <sge/cegui/to_cegui_string.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/from_cegui_string.hpp>
#include <sge/cegui/toolbox/append_row.hpp>
#include <sge/cegui/toolbox/row.hpp>
#include <sge/config/cache_path.hpp>
#include <sge/systems/instance.hpp>
#include <CEGUIEvent.h>
#include <CEGUIString.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/string.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/lexical_cast.hpp>
#include <fcppt/to_std_string.hpp>
#include <fcppt/from_std_string.hpp>
#include <boost/range/algorithm/sort.hpp>

#include "../../highscore/score.hpp"
#include "../../name.hpp"
#include "../../../fruitlib/json/find_and_convert_member.hpp"
#include "../../../fruitlib/json/convert_from.hpp"
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

namespace
{
struct highscore_entry
{
public:
	fcppt::string name;
	fruitcut::app::highscore::score::value_type score_;
	boost::posix_time::ptime date_time;

	explicit
	highscore_entry(
		fcppt::string const &_name,
		fruitcut::app::highscore::score::value_type const _score,
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
		sge::config::cache_path(
			fruitcut::app::name())/
		FCPPT_TEXT("highscore.json");

	if(!fcppt::filesystem::exists(highscore_file_path))
		return highscore_sequence();

	sge::parse::json::array const json_file = 
		fruitcut::fruitlib::json::find_and_convert_member<sge::parse::json::array>(
			sge::parse::json::parse_file_exn(
				highscore_file_path),
			fruitcut::fruitlib::json::path(
				FCPPT_TEXT("entries")));

	highscore_sequence result;
	
	typedef
	std::vector<sge::parse::json::object> 
	json_object_vector;

	json_object_vector const json_objects(
		(fruitcut::fruitlib::json::convert_from<json_object_vector>(
			json_file)));

	for(
		json_object_vector::const_iterator current_entry = 
			json_objects.begin();
		current_entry != json_objects.end();
		++current_entry)
	{
		result.push_back(
			highscore_entry(
				fruitcut::fruitlib::json::find_and_convert_member<sge::parse::json::string>(
					*current_entry,
					fruitcut::fruitlib::json::path(
						FCPPT_TEXT("name"))),
				fruitcut::fruitlib::json::find_and_convert_member<fruitcut::app::highscore::score::value_type>(
					*current_entry,
					fruitcut::fruitlib::json::path(
						FCPPT_TEXT("score"))),
				boost::posix_time::from_iso_string(
					fcppt::to_std_string(
						fruitcut::fruitlib::json::find_and_convert_member<sge::parse::json::string>(
							*current_entry,
							fruitcut::fruitlib::json::path(
								FCPPT_TEXT("date-time")))))));
	}

	return result;
}

void
write_highscore(
	highscore_sequence const &entries)
{
	fcppt::filesystem::path const highscore_path = 
		sge::config::cache_path(
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
			fruitcut::app::exception(
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
		context<machine>().gui_system(),
		media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("layouts")/FCPPT_TEXT("highscore.layout"),
		context<machine>().systems().charconv_system()),
	gui_sheet_(
		context<machine>().gui_system(),
		*context<machine>().gui_system().window_manager().getWindow("Highscore")),
	quit_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Highscore/Quit")),
	reset_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Highscore/Reset")),
	main_menu_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"Highscore/MainMenu")),
	quit_button_connection_(
		quit_button_.push_callback(
			std::tr1::bind(
				&app::machine::quit,
				&context<app::machine>()))),
	reset_button_connection_(
		reset_button_.push_callback(
			FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				ingame::superstate))),
	main_menu_button_connection_(
		main_menu_button_.push_callback(
			FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main)))
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
	for(
		highscore_sequence::const_iterator current_entry = 
			entries.begin();
		current_entry != entries.end();
		++current_entry)
	{
		typedef 
		boost::date_time::c_local_adjustor<boost::posix_time::ptime> 
		local_adjuster;

		sge::cegui::toolbox::append_row(
			dynamic_cast<CEGUI::MultiColumnList &>(
				*context<machine>().gui_system().window_manager().getWindow("Highscore/List")),
			fcppt::assign::make_container<sge::cegui::toolbox::row>
				(sge::cegui::to_cegui_string(
					fcppt::lexical_cast<fcppt::string>(
						index),
					context<machine>().systems().charconv_system()))
				(sge::cegui::to_cegui_string(
					current_entry->name,
					context<machine>().systems().charconv_system()))
				(sge::cegui::to_cegui_string(
					fcppt::lexical_cast<fcppt::string>(
						current_entry->score_),
					context<machine>().systems().charconv_system()))
				(sge::cegui::to_cegui_string(
					fcppt::from_std_string(
						boost::posix_time::to_simple_string(
							local_adjuster::utc_to_local(
								current_entry->date_time))),
					context<machine>().systems().charconv_system())));
	}
}

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::main,
	gameover::highscore)

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	ingame::superstate,
	gameover::highscore)

fruitcut::app::states::gameover::highscore::~highscore()
{
}
