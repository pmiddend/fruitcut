#include <fruitapp/exception.hpp>
#include <fruitapp/name.hpp>
#include <fruitapp/highscore/providers_from_json.hpp>
#include <fruitapp/highscore/provider/file/object.hpp>
#include <fruitapp/highscore/provider/net/object.hpp>
#include <sge/config/app_name.hpp>
#include <sge/config/cache_path.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/convert_from.hpp>
#include <sge/parse/json/element_vector.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/filesystem/path_to_string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>
#include <fcppt/config/external_end.hpp>


void
fruitapp::highscore::providers_from_json(
	sge::parse::json::array const &_json_providers,
	highscore::provider_sequence &result)
{
	for(
		sge::parse::json::element_vector::const_iterator current_element =
			_json_providers.elements.begin();
		current_element != _json_providers.elements.end();
		++current_element)
	{
		fcppt::string const uri =
			sge::parse::json::find_and_convert_member<fcppt::string>(
				sge::parse::json::convert_from<sge::parse::json::object>(
					*current_element),
				sge::parse::json::path(
					FCPPT_TEXT("url")));

		fcppt::string protocol,address;
		unsigned port = 31337;

		namespace qi = boost::spirit::qi;

		fcppt::string::const_iterator current_position =
			uri.begin();

		typedef
		qi::as<fcppt::string>
		as_fcppt_string_type;

		as_fcppt_string_type const as_fcppt_string =
			as_fcppt_string_type();

		bool const parse_result =
			qi::parse(
				current_position,
				uri.end(),
				(as_fcppt_string[+~qi::standard_wide::char_(FCPPT_TEXT(':'))])[boost::phoenix::ref(protocol) = qi::_1] >>
				qi::lit(FCPPT_TEXT("://")) >>
				(
					((as_fcppt_string[+~qi::standard_wide::char_(FCPPT_TEXT(':'))])[boost::phoenix::ref(address) = qi::_1] >> qi::lit(FCPPT_TEXT(':')) >> qi::uint_[boost::phoenix::ref(port) = qi::_1]) |
					(as_fcppt_string[+qi::standard_wide::char_])[boost::phoenix::ref(address) = qi::_1]));

		if(!parse_result || current_position != uri.end())
			throw
				fruitapp::exception(
					FCPPT_TEXT("Provider URI \"")+uri+FCPPT_TEXT("\" is invalid"));

		if(protocol == FCPPT_TEXT("file"))
			address =
				boost::algorithm::replace_all_copy(
					address,
					FCPPT_TEXT("$CACHE_PATH"),
					fcppt::filesystem::path_to_string(
						sge::config::cache_path(
							sge::config::app_name(
								fruitapp::name()))));

		if(protocol == FCPPT_TEXT("file"))
		{
			result.push_back(
				fcppt::make_unique_ptr<provider::file::object>(
					address));
		}
		else if(protocol == FCPPT_TEXT("fruitcut"))
		{
			result.push_back(
				fcppt::make_unique_ptr<provider::net::object>(
					provider::net::host(
						address),
					provider::net::port(
						port)));
		}
		else
			throw fruitapp::exception(FCPPT_TEXT("Unsupported highscore protocol \"")+protocol+FCPPT_TEXT("\""));
	}
}
