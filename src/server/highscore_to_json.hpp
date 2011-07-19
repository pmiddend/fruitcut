#ifndef FRUITCUT_SERVER_HIGHSCORE_TO_JSON_HPP_INCLUDED
#define FRUITCUT_SERVER_HIGHSCORE_TO_JSON_HPP_INCLUDED

#include "highscore_sequence.hpp"
#include "ascii/string.hpp"
#include <string>

namespace fruitcut
{
namespace server
{
ascii::string const
highscore_to_json(
	server::highscore_sequence const &);
}
}

#endif