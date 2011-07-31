#ifndef FRUITSERVER_HIGHSCORE_TO_JSON_HPP_INCLUDED
#define FRUITSERVER_HIGHSCORE_TO_JSON_HPP_INCLUDED

#include "highscore_sequence.hpp"
#include "ascii/string.hpp"
#include <string>

namespace fruitserver
{
ascii::string const
highscore_to_json(
	fruitserver::highscore_sequence const &);
}

#endif
