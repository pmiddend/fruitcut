#include "connection.hpp"
#include "../../json_to_entry_set.hpp"
#include "../../../../fruitlib/utf8_file_to_fcppt_string.hpp"
#include <sge/parse/json/json.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/filesystem/path_to_string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iterator>

fruitapp::highscore::provider::file::connection::connection(
	fcppt::filesystem::path const &_path)
:
	path_(
		_path),
	message_received_(),
	error_received_(),
	list_received_(),
	rank_received_()
{
}

void
fruitapp::highscore::provider::file::connection::post_rank(
	highscore::name const &_name,
	highscore::score const &_score)
{
	sge::parse::json::object result;
	fcppt::string const converted_file = 
		fruitlib::utf8_file_to_fcppt_string(
			path_);
	fcppt::string::const_iterator current_position = 
		converted_file.begin();
	if(!sge::parse::json::parse_range(current_position,converted_file.end(),result))
	{
		error_received_(
			FCPPT_TEXT("Couldn't parse file \"")+
			fcppt::filesystem::path_to_string(
				path_)+
			FCPPT_TEXT("\""));
		return;
	}

	highscore::entry_set entries = 
		highscore::json_to_entry_set(
			result);

	highscore::entry const new_entry(
		_name,
		_score,
		boost::posix_time::ptime(
			boost::posix_time::second_clock::universal_time()));

	entries.insert(
		new_entry);

	highscore::entry_set::const_iterator const place = 
		entries.find(
			new_entry);

	rank_received_(
		highscore::rank(
			static_cast<highscore::rank::value_type>(
				std::distance(
					entries.begin(),
					place))));
}

void
fruitapp::highscore::provider::file::connection::retrieve_list()
{
	sge::parse::json::object result;
	if(!sge::parse::json::parse_file(path_,result))
	{
		error_received_(
			FCPPT_TEXT("Couldn't parse file \"")+
			fcppt::filesystem::path_to_string(
				path_)+
			FCPPT_TEXT("\""));
		return;
	}

	list_received_(
		highscore::json_to_entry_set(
			result));

	message_received_(
		FCPPT_TEXT("Parsed file \"")+
		fcppt::filesystem::path_to_string(
			path_)+
		FCPPT_TEXT("\" successfully"));
}

void
fruitapp::highscore::provider::file::connection::update()
{
}

fcppt::signal::auto_connection
fruitapp::highscore::provider::file::connection::message_received(
	callbacks::message_received const &f)
{
	return 
		message_received_.connect(
			f);
}

fcppt::signal::auto_connection
fruitapp::highscore::provider::file::connection::error_received(
	callbacks::error_received const &f)
{
	return 
		error_received_.connect(
			f);
}

fcppt::signal::auto_connection
fruitapp::highscore::provider::file::connection::list_received(
	callbacks::list_received const &f)
{
	return 
		list_received_.connect(
			f);
}

fcppt::signal::auto_connection
fruitapp::highscore::provider::file::connection::rank_received(
	callbacks::rank_received const &f)
{
	return 
		rank_received_.connect(
			f);
}

fruitapp::highscore::provider::file::connection::~connection()
{
}
