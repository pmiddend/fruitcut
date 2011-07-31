#ifndef FRUITSERVER_ASCII_FROM_BYTE_SEQUENCE_HPP_INCLUDED
#define FRUITSERVER_ASCII_FROM_BYTE_SEQUENCE_HPP_INCLUDED

#include "string.hpp"
#include "../byte_sequence.hpp"

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
