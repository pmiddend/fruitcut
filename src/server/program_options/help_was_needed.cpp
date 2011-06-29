#include "help_was_needed.hpp"

fruitcut::server::program_options::help_was_needed::help_was_needed(
	std::string const &_help_string)
:
	help_string_(
		_help_string)
{
}

std::string const &
fruitcut::server::program_options::help_was_needed::help_string() const
{
	return 
		help_string_;
}
