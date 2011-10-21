#include <fcppt/config/external_begin.hpp>
#include <fruitserver/program_options/help_was_needed.hpp>
#include <fcppt/config/external_end.hpp>


fruitserver::program_options::help_was_needed::help_was_needed(
	std::string const &_help_string)
:
	help_string_(
		_help_string)
{
}

std::string const &
fruitserver::program_options::help_was_needed::help_string() const
{
	return
		help_string_;
}
