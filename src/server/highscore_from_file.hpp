#ifndef FRUITCUT_SERVER_HIGHSCORE_FROM_FILE_HPP_INCLUDED
#define FRUITCUT_SERVER_HIGHSCORE_FROM_FILE_HPP_INCLUDED

#include "highscore_sequence.hpp"
#include <string>

namespace fruitcut
{
namespace server
{
server::highscore_sequence const 
highscore_from_file(
	std::string const &);
}
}

#endif
