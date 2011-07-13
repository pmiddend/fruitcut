#ifndef FRUITCUT_SERVER_PROCESS_COMMAND_HPP_INCLUDED
#define FRUITCUT_SERVER_PROCESS_COMMAND_HPP_INCLUDED

#include "command.hpp"
#include <ostream>
#include <string>

namespace fruitcut
{
namespace server
{
std::string
process_command(
	std::ostream &,
	server::command const &,
	std::string const &data_dir);
}
}

#endif
