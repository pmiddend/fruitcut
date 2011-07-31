#ifndef FRUITSERVER_ASCII_TO_BYTE_SEQUENCE_HPP_INCLUDED
#define FRUITSERVER_ASCII_TO_BYTE_SEQUENCE_HPP_INCLUDED

#include <fruitserver/byte_sequence.hpp>
#include <fruitserver/ascii/string.hpp>

namespace fruitserver
{
namespace ascii
{
fruitserver::byte_sequence const
to_byte_sequence(
	ascii::string const &);
}
}

#endif
