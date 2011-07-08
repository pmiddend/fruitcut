#include "providers_from_json.hpp"
#include "../name.hpp"
#include "../../fruitlib/json/find_and_convert_member.hpp"
#include "../../fruitlib/json/convert_from.hpp"
#include <sge/parse/json/json.hpp>
#include <sge/config/find_config_path.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>

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
		fcppt::string const url = 
			fruitlib::json::find_and_convert_member<fcppt::string>(
				fruitlib::json::convert_from<sge::parse::json::object>(
					*current_element),
				fruitlib::json::path(
					FCPPT_TEXT("url")));

		/*
			boost::algorithm::replace_all_copy(
				a,
				FCPPT_TEXT("$CACHE_PATH"),
				sge::config::find_cache_path(
					app::name()));
		*/
	}
}
