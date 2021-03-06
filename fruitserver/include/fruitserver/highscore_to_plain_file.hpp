#ifndef FRUITSERVER_HIGHSCORE_TO_PLAIN_FILE_HPP_INCLUDED
#define FRUITSERVER_HIGHSCORE_TO_PLAIN_FILE_HPP_INCLUDED

#include <fruitserver/highscore_sequence.hpp>
#include <fruitserver/path.hpp>


namespace fruitserver
{
void
highscore_to_plain_file(
	fruitserver::highscore_sequence const &,
	fruitserver::path const &);
}

#endif
