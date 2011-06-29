#include "make_command_line_parameters.hpp"
#include "command_line_parameters.hpp"

fruitcut::server::program_options::command_line_parameters const
fruitcut::server::program_options::make_command_line_parameters(
	int argc,
	char *argv[])
{
	fruitcut::server::program_options::command_line_parameters result;
	for(int i = 1; i < argc; ++i)
		result.push_back(
			argv[i]);
	return 
		result;
}
