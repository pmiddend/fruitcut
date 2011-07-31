#include <fruitlib/json/string_to_path.hpp>
#include <fruitlib/json/path.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <fcppt/text.hpp>

fruitlib::json::path const
fruitlib::json::string_to_path(
	fcppt::string const &input_path)
{
	json::path::sequence_type parts;
	
	boost::algorithm::split(
		parts,
		input_path,
		boost::algorithm::is_any_of(
			FCPPT_TEXT("/")));

	return 
		json::path(
			parts.begin(),
			parts.end());
}
