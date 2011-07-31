#include <fruitlib/json/path_to_string.hpp>
#include <fruitlib/json/path.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>

fcppt::string const
fruitlib::json::path_to_string(
	json::path const &p)
{
	fcppt::string result;
	for(json::path::const_iterator i = p.begin(); i != p.end(); ++i)
	{
		result += *i;
		if(i != --p.end())
			result += FCPPT_TEXT("/");
	}
	return result;
}
