#ifndef FRUITLIB_FONT_IDENTIFIER_HPP_INCLUDED
#define FRUITLIB_FONT_IDENTIFIER_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <fcppt/strong_typedef.hpp>

namespace fruitlib
{
namespace font
{
// This currently is just a string but it could be extended to
// something more useful (like a smart pointer)
FCPPT_MAKE_STRONG_TYPEDEF(
	fcppt::string,
	identifier);
}
}

#endif
