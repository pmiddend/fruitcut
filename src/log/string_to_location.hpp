#ifndef FRUITCUT_LOG_STRING_TO_LOCATION_HPP_INCLUDED
#define FRUITCUT_LOG_STRING_TO_LOCATION_HPP_INCLUDED

#include <fcppt/log/location.hpp>
#include <fcppt/string.hpp>
#include <fcppt/char_type.hpp>

namespace fruitcut
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
