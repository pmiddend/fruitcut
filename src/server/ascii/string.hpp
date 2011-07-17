#ifndef FRUITCUT_SERVER_ASCII_STRING_HPP_INCLUDED
#define FRUITCUT_SERVER_ASCII_STRING_HPP_INCLUDED

#include "char.hpp"
#include "char_traits.hpp"
#include <string>

namespace fruitcut
{
namespace server
{
namespace ascii
{
typedef
std::basic_string
<
	ascii::char_,
	ascii::char_traits
>
string;
}
}
}

#endif
