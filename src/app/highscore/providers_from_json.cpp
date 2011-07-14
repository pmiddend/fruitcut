#include "providers_from_json.hpp"
#include "provider/file/object.hpp"
#include "provider/net/object.hpp"
#include "../name.hpp"
#include "../exception.hpp"
#include "../../fruitlib/json/find_and_convert_member.hpp"
#include "../../fruitlib/json/convert_from.hpp"
#include <sge/parse/json/json.hpp>
#include <sge/config/cache_path.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <fcppt/filesystem/path_to_string.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <iostream>

void
fruitcut::app::highscore::providers_from_json(
	sge::parse::json::object const &config_file,
	highscore::provider_sequence &result)
{
	sge::parse::json::array const &providers = 
		fruitlib::json::find_and_convert_member<sge::parse::json::array>(
			config_file,
			fruitlib::json::path(
				FCPPT_TEXT("highscore-providers")));

	for(
		sge::parse::json::element_vector::const_iterator current_element = 
			providers.elements.begin();
		current_element != providers.elements.end();
		++current_element)
	{
		fcppt::string const uri = 
			fruitlib::json::find_and_convert_member<fcppt::string>(
				fruitlib::json::convert_from<sge::parse::json::object>(
					*current_element),
				fruitlib::json::path(
					FCPPT_TEXT("url")));

		fcppt::string protocol,address;
		unsigned port = 31337;

		namespace qi = boost::spirit::qi;

		fcppt::string::const_iterator current_position = 
			uri.begin();

		bool const parse_result = 
			qi::parse(
				current_position,
				uri.end(),
				(qi::as_string[+~qi::standard_wide::char_(FCPPT_TEXT(':'))])[boost::phoenix::ref(protocol) = qi::_1] >> 
				qi::lit("://") >> 
				(
					((qi::as_string[+~qi::char_(FCPPT_TEXT(':'))])[boost::phoenix::ref(address) = qi::_1] >> qi::lit(FCPPT_TEXT(':')) >> qi::uint_[boost::phoenix::ref(port) = qi::_1]) | 
					(qi::as_string[+qi::standard_wide::char_])[boost::phoenix::ref(address) = qi::_1]));

		if(!parse_result || current_position != uri.end())
			throw 
				app::exception(
					FCPPT_TEXT("Provider URI \"")+uri+FCPPT_TEXT("\" is invalid"));

		if(protocol == FCPPT_TEXT("file"))
			address = 
				boost::algorithm::replace_all_copy(
					address,
					FCPPT_TEXT("$CACHE_PATH"),
					fcppt::filesystem::path_to_string(
						sge::config::cache_path(
							app::name())));

		if(protocol == FCPPT_TEXT("file"))
		{
			fcppt::container::ptr::push_back_unique_ptr(
				result,
				fcppt::make_unique_ptr<provider::file::object>(
					address));
		}
		else if(protocol == FCPPT_TEXT("fruitcut"))
		{
			fcppt::container::ptr::push_back_unique_ptr(
				result,
				fcppt::make_unique_ptr<provider::net::object>(
					provider::net::host(
						address),
					provider::net::port(
						port)));
		}
		else
			throw app::exception(FCPPT_TEXT("Unsupported highscore protocol \"")+protocol+FCPPT_TEXT("\""));
	}
}
