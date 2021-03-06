#ifndef FRUITSERVER_ASCII_STRING_HPP_INCLUDED
#define FRUITSERVER_ASCII_STRING_HPP_INCLUDED

#include <fruitserver/ascii/char.hpp>
#include <fruitserver/ascii/char_traits.hpp>
#include <string>


namespace fruitserver
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

#endif
