#ifndef FRUITSERVER_PROGRAM_OPTIONS_MAKE_COMMAND_LINE_PARAMETERS_HPP_INCLUDED
#define FRUITSERVER_PROGRAM_OPTIONS_MAKE_COMMAND_LINE_PARAMETERS_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <fruitserver/program_options/command_line_parameters.hpp>
#include <fcppt/config/external_end.hpp>


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
