#ifndef FRUITSERVER_PROGRAM_OPTIONS_MAKE_COMMAND_LINE_PARAMETERS_HPP_INCLUDED
#define FRUITSERVER_PROGRAM_OPTIONS_MAKE_COMMAND_LINE_PARAMETERS_HPP_INCLUDED

#include <fruitserver/program_options/command_line_parameters.hpp>

namespace fruitserver
{
namespace program_options
{
command_line_parameters const
make_command_line_parameters(
	int argc,
	char *argv[]);
}
}

#endif
