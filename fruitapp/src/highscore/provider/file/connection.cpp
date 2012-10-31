#include <fruitapp/highscore/entry_set_to_json.hpp>
#include <sge/parse/result.hpp>
#include <fruitapp/highscore/json_to_entry_set.hpp>
#include <fruitapp/highscore/provider/file/connection.hpp>
#include <fruitlib/fcppt_string_to_utf8_file.hpp>
#include <fruitlib/utf8_file_to_fcppt_string.hpp>
#include <sge/charconv/system_fwd.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_file.hpp>
#include <sge/parse/json/parse_range.hpp>
#include <sge/parse/json/output/tabbed_to_string.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/filesystem/path_to_string.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <iterator>
#include <fcppt/config/external_end.hpp>


fruitapp::highscore::provider::file::connection::connection(
	sge::charconv::system &_charconv_system,
	boost::filesystem::path const &_path)
:
	path_(
		_path),
	charconv_system_(
		_charconv_system),
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
	boost::filesystem::path const directory_path(
		path_.parent_path());

	// Speculatively create directories, deliberately ignoring the return
	// value. If all directories exist, this will return false (which isn't
	// exactly an error for us).
	boost::filesystem::create_directories(
		directory_path);

	// Try to open the file and read its contents.
	fcppt::optional<fcppt::string> const converted_file(
		fruitlib::utf8_file_to_fcppt_string(
			charconv_system_,
			path_));

	// ... and create highscore entries from it
	highscore::entry_set entries;

	// This branch is followed if we were able to read from the file. If
	// the file doesn't exist, we don't follow this branch.
	if(converted_file)
	{
		fcppt::string::const_iterator current_position =
			converted_file->begin();

		sge::parse::json::start result;

		// If the file exists, it has to be valid. It won't be
		// overridden because it's invalid (we might want to inspect
		// _why_ it's invalid instead of creating a new file).
		if(!sge::parse::json::parse_range(current_position,converted_file->end(),result))
		{
			error_received_(
				FCPPT_TEXT("Couldn't parse file \"")+
				fcppt::filesystem::path_to_string(
					path_)+
				FCPPT_TEXT("\""));
			return;
		}

		entries =
			highscore::json_to_entry_set(
				result.object());
	}

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

	if(
		!fruitlib::fcppt_string_to_utf8_file(
			sge::parse::json::output::tabbed_to_string(
				sge::parse::json::start(
					highscore::entry_set_to_json(
						entries))),
			path_,
			charconv_system_))
	{
		error_received_(
			FCPPT_TEXT("Couldn't create the highscore file \"")+
			fcppt::filesystem::path_to_string(
				path_)+
			FCPPT_TEXT("\""));
		return;
	}

	rank_received_(
		highscore::rank(
			static_cast<highscore::rank::value_type>(
				std::distance(
					entries.begin(),
					place)+1)));
}

void
fruitapp::highscore::provider::file::connection::retrieve_list()
{
	sge::parse::json::start result;

	sge::parse::result const parse_result(
		sge::parse::json::parse_file(
			path_,
			result));

	if(parse_result.result_code() != sge::parse::result_code::ok)
	{
		if(parse_result.error_string().has_value())
			error_received_(
				FCPPT_TEXT("Couldn't parse file \"")+
				fcppt::filesystem::path_to_string(
					path_)+
				FCPPT_TEXT("\": ")+
				((*parse_result.error_string()).get()));
		else
			error_received_(
				FCPPT_TEXT("Couldn't parse file \"")+
				fcppt::filesystem::path_to_string(
					path_)+
				FCPPT_TEXT("\": Unknown reason"));
		return;
	}

	list_received_(
		highscore::json_to_entry_set(
			result.object()));

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
