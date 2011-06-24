#include "create_command_line_parameters.hpp"
#include "command_line_parameters.hpp"
#include <fcppt/from_std_string.hpp>
#include <cstddef>

fruitcut::fruitlib::command_line_parameters const
fruitcut::fruitlib::create_command_line_parameters(
	int const argc,
	char *argv[])
{
	fruitlib::command_line_parameters result(
		static_cast<fruitlib::command_line_parameters::size_type>(
			argc));
	for(
		fruitlib::command_line_parameters::size_type i = 
			0; 
		i < static_cast<fruitlib::command_line_parameters::size_type>(argc); 
		++i)
		result[i] = 
			fcppt::from_std_string(
				argv[static_cast<std::size_t>(i)]);
	return result;
}
