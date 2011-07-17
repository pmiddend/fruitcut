#ifndef FRUITCUT_SERVER_ASCII_TO_BYTE_SEQUENCE_HPP_INCLUDED
#define FRUITCUT_SERVER_ASCII_TO_BYTE_SEQUENCE_HPP_INCLUDED

#include "../byte_sequence.hpp"
#include "string.hpp"

namespace fruitcut
{
namespace server
{
namespace ascii
{
server::byte_sequence const
to_byte_sequence(
	ascii::string const &);
}
}
}

#endif
