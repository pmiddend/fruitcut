#ifndef FRUITLIB_LOG_STRING_TO_LOCATION_HPP_INCLUDED
#define FRUITLIB_LOG_STRING_TO_LOCATION_HPP_INCLUDED

#include <fcppt/char_type.hpp>
#include <fcppt/string.hpp>
#include <fcppt/log/location.hpp>


namespace fruitlib
{
namespace log
{
fcppt::log::location const
string_to_location(
	fcppt::string const &,
	fcppt::char_type delimiter);
}
}

#endif
