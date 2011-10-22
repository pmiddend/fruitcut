#ifndef FRUITSERVER_HIGHSCORE_FROM_PLAIN_FILE_HPP_INCLUDED
#define FRUITSERVER_HIGHSCORE_FROM_PLAIN_FILE_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <fruitserver/highscore_sequence.hpp>
#include <fruitserver/path.hpp>
#include <string>
#include <fcppt/config/external_end.hpp>


namespace fruitserver
{
fruitserver::highscore_sequence const
highscore_from_plain_file(
	fruitserver::path const &);
}

#endif
