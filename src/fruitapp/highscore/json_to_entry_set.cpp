#include <fruitapp/highscore/json_to_entry_set.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/convert_from.hpp>
#include <sge/parse/json/path.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <fcppt/text.hpp>
#include <fcppt/to_std_string.hpp>
#include <fcppt/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <vector>

fruitapp::highscore::entry_set const
fruitapp::highscore::json_to_entry_set(
	sge::parse::json::object const &input_json)
{
	sge::parse::json::array const json_file =
		sge::parse::json::find_and_convert_member<sge::parse::json::array>(
			input_json,
			sge::parse::json::path(
				FCPPT_TEXT("entries")));

	highscore::entry_set result;

	typedef
	std::vector<sge::parse::json::object>
	json_object_vector;

	json_object_vector const json_objects(
		(sge::parse::json::convert_from<json_object_vector>(
			json_file)));

	for(
		json_object_vector::const_iterator current_entry =
			json_objects.begin();
		current_entry != json_objects.end();
		++current_entry)
		result.insert(
			highscore::entry(
				highscore::name(
					sge::parse::json::find_and_convert_member<fcppt::string>(
						*current_entry,
						sge::parse::json::path(
							FCPPT_TEXT("name")))),
				highscore::score(
					sge::parse::json::find_and_convert_member<highscore::score::value_type>(
						*current_entry,
						sge::parse::json::path(
							FCPPT_TEXT("score")))),
				boost::posix_time::from_iso_string(
					fcppt::to_std_string(
						sge::parse::json::find_and_convert_member<fcppt::string>(
							*current_entry,
							sge::parse::json::path(
								FCPPT_TEXT("date-time")))))));

	return result;
}
