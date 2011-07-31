#ifndef FRUITLIB_JSON_MERGE_COMMAND_LINE_PARAMETERS_HPP_INCLUDED
#define FRUITLIB_JSON_MERGE_COMMAND_LINE_PARAMETERS_HPP_INCLUDED

#include <fruitlib/command_line_parameters.hpp>
#include <sge/parse/json/object.hpp>

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

#endif
