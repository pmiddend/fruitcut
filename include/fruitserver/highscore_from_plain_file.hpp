#ifndef FRUITSERVER_HIGHSCORE_FROM_PLAIN_FILE_HPP_INCLUDED
#define FRUITSERVER_HIGHSCORE_FROM_PLAIN_FILE_HPP_INCLUDED

#include <fruitserver/highscore_sequence.hpp>
#include <fruitserver/path.hpp>
#include <string>

namespace fruitserver
{
fruitserver::highscore_sequence const
highscore_from_plain_file(
	fruitserver::path const &);
}

#endif
