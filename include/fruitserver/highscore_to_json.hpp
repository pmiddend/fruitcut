#ifndef FRUITSERVER_HIGHSCORE_TO_JSON_HPP_INCLUDED
#define FRUITSERVER_HIGHSCORE_TO_JSON_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <fruitserver/highscore_sequence.hpp>
#include <fruitserver/ascii/string.hpp>
#include <string>
#include <fcppt/config/external_end.hpp>


namespace fruitserver
{
ascii::string const
highscore_to_json(
	fruitserver::highscore_sequence const &);
}

#endif
