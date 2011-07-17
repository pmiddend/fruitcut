#ifndef FRUITCUT_SERVER_ASCII_FROM_BYTE_SEQUENCE_HPP_INCLUDED
#define FRUITCUT_SERVER_ASCII_FROM_BYTE_SEQUENCE_HPP_INCLUDED

#include "string.hpp"
#include "../byte_sequence.hpp"

namespace fruitcut
{
namespace server
{
namespace ascii
{
ascii::string const
from_byte_sequence(
	server::byte_sequence const &);
}
}
}

#endif
