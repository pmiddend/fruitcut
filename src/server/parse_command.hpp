#ifndef FRUITCUT_SERVER_PARSE_COMMAND_HPP_INCLUDED
#define FRUITCUT_SERVER_PARSE_COMMAND_HPP_INCLUDED

#include "command.hpp"
#include "ascii/string.hpp"

namespace fruitcut
{
namespace server
{
server::command const
parse_command(
	ascii::string const &);
}
}

#endif
