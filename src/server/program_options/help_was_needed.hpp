#ifndef FRUITCUT_SERVER_PROGRAM_OPTIONS_HELP_WAS_NEEDED_HPP_INCLUDED
#define FRUITCUT_SERVER_PROGRAM_OPTIONS_HELP_WAS_NEEDED_HPP_INCLUDED

#include <string>

namespace fruitcut
{
namespace server
{
namespace program_options
{
class help_was_needed
{
public:
	explicit
	help_was_needed(
		std::string const &);

	std::string const &
	help_string() const;
private:
	std::string help_string_;
};
}
}
}

#endif
