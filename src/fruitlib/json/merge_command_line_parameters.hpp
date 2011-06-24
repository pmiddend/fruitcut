#ifndef FRUITCUT_FRUITLIB_JSON_MERGE_COMMAND_LINE_PARAMETERS_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_JSON_MERGE_COMMAND_LINE_PARAMETERS_HPP_INCLUDED

#include "../command_line_parameters.hpp"
#include <sge/parse/json/object.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace json
{
sge::parse::json::object const
merge_command_line_parameters(
	sge::parse::json::object,
	fruitlib::command_line_parameters const &);
}
}
}

#endif
