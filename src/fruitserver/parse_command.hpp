#ifndef FRUITSERVER_PARSE_COMMAND_HPP_INCLUDED
#define FRUITSERVER_PARSE_COMMAND_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <fruitserver/command.hpp>
#include <fruitserver/ascii/string.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitserver
{
fruitserver::command const
parse_command(
	ascii::string const &);
}

#endif
