#ifndef FRUITCUT_JSON_STRING_TO_VALUE_HPP_INCLUDED
#define FRUITCUT_JSON_STRING_TO_VALUE_HPP_INCLUDED

#include <sge/parse/json/value.hpp>
#include <fcppt/string.hpp>

namespace fruitcut
{
namespace json
{
sge::parse::json::value const
string_to_value(
	fcppt::string const &);
}
}

#endif
