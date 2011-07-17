#ifndef FRUITCUT_SERVER_HIGHSCORE_TO_PLAIN_FILE_HPP_INCLUDED
#define FRUITCUT_SERVER_HIGHSCORE_TO_PLAIN_FILE_HPP_INCLUDED

#include "highscore_sequence.hpp"
#include "path.hpp"

namespace fruitcut
{
namespace server
{
void
highscore_to_plain_file(
	server::highscore_sequence const &,
	server::path const &);
}
}

#endif
