#include "parse_string_exn.hpp"
#include <sge/parse/json/parse_range.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>

sge::parse::json::object const
fruitlib::json::parse_string_exn(
	fcppt::string const &s)
{
	sge::parse::json::object result;
	fcppt::string::const_iterator current = 
		s.begin();
	if(!sge::parse::json::parse_range(current,s.end(),result))
		throw	
			sge::parse::json::exception(
				FCPPT_TEXT("Error parsing json"));
	return result;
}
