#ifndef FRUITSERVER_HIGHSCORE_TO_PLAIN_FILE_HPP_INCLUDED
#define FRUITSERVER_HIGHSCORE_TO_PLAIN_FILE_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <fruitserver/highscore_sequence.hpp>
#include <fruitserver/path.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitserver
{
void
highscore_to_plain_file(
	fruitserver::highscore_sequence const &,
	fruitserver::path const &);
}

#endif
