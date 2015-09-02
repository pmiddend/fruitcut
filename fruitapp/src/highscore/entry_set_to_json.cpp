#include <fruitapp/highscore/entry_set_to_json.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/value.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/variant/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <fcppt/config/external_end.hpp>


sge::parse::json::object const
fruitapp::highscore::entry_set_to_json(
	highscore::entry_set const &_entries)
{
	sge::parse::json::array entry_array;

	for(
		highscore::entry_set::const_iterator it =
			_entries.begin();
		it != _entries.end();
		++it)
	{
		sge::parse::json::object json_entry;

		json_entry.members.insert(
			sge::parse::json::member_map::value_type(
				FCPPT_TEXT("name"),
				sge::parse::json::value(
					it->name())));

		json_entry.members.insert(
			sge::parse::json::member_map::value_type(
				FCPPT_TEXT("score"),
				sge::parse::json::value(
					static_cast<sge::parse::json::int_type>(
						it->score()))));

		json_entry.members.insert(
			sge::parse::json::member_map::value_type(
				FCPPT_TEXT("date-time"),
				sge::parse::json::value(
					fcppt::from_std_string(
						boost::posix_time::to_iso_string(
							it->date_time())))));

		entry_array.elements.push_back(
			sge::parse::json::value(
				json_entry));
	}

	sge::parse::json::object result;

	result.members.insert(
		sge::parse::json::member_map::value_type(
			FCPPT_TEXT("entries"),
			sge::parse::json::value(
				entry_array)));

	return result;
}
