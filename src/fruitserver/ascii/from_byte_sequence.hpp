#ifndef FRUITSERVER_ASCII_FROM_BYTE_SEQUENCE_HPP_INCLUDED
#define FRUITSERVER_ASCII_FROM_BYTE_SEQUENCE_HPP_INCLUDED

#include <fruitserver/byte_sequence.hpp>
#include <fruitserver/ascii/string.hpp>


namespace fruitserver
{
namespace ascii
{
ascii::string const
from_byte_sequence(
	fruitserver::byte_sequence const &);
}
}

#endif
