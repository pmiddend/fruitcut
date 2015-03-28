#ifndef FRUITLIB_CREATE_COMMAND_LINE_PARAMETERS_HPP_INCLUDED
#define FRUITLIB_CREATE_COMMAND_LINE_PARAMETERS_HPP_INCLUDED

#include <fruitlib/command_line_parameters.hpp>
#include <fruitlib/detail/symbol.hpp>


namespace fruitlib
{
FRUITLIB_DETAIL_SYMBOL
fruitlib::command_line_parameters const
create_command_line_parameters(
	int argc,
	char *argv[]);
}

#endif
