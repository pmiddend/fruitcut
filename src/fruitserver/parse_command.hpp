#ifndef FRUITSERVER_PARSE_COMMAND_HPP_INCLUDED
#define FRUITSERVER_PARSE_COMMAND_HPP_INCLUDED

#include <fruitserver/command.hpp>
#include <fruitserver/ascii/string.hpp>


namespace fruitserver
{
fruitserver::command const
parse_command(
	ascii::string const &);
}

#endif
