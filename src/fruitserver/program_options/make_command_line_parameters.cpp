#include <fruitserver/program_options/make_command_line_parameters.hpp>
#include <fruitserver/program_options/command_line_parameters.hpp>

fruitserver::program_options::command_line_parameters const
fruitserver::program_options::make_command_line_parameters(
	int argc,
	char *argv[])
{
	fruitserver::program_options::command_line_parameters result;
	for(int i = 1; i < argc; ++i)
		result.push_back(
			argv[i]);
	return
		result;
}
