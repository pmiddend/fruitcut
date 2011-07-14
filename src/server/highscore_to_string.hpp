#ifndef FRUITCUT_SERVER_HIGHSCORE_TO_STRING_HPP_INCLUDED
#define FRUITCUT_SERVER_HIGHSCORE_TO_STRING_HPP_INCLUDED

#include "highscore_sequence.hpp"
#include <string>

namespace fruitcut
{
namespace server
{
std::string
highscore_to_string(
	server::highscore_sequence const &);
}
}

#endif
