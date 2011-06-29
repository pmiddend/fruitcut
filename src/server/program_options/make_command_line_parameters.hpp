#ifndef FRUITCUT_SERVER_PROGRAM_OPTIONS_MAKE_COMMAND_LINE_PARAMETERS_HPP_INCLUDED
#define FRUITCUT_SERVER_PROGRAM_OPTIONS_MAKE_COMMAND_LINE_PARAMETERS_HPP_INCLUDED

#include "command_line_parameters.hpp"

namespace fruitcut
{
namespace server
{
namespace program_options
{
command_line_parameters const
make_command_line_parameters(
	int argc,
	char *argv[]);
}
}
}

#endif
