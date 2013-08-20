#ifndef FRUITLIB_HUMAN_READABLE_TIME_DIFFERENCE_HPP_INCLUDED
#define FRUITLIB_HUMAN_READABLE_TIME_DIFFERENCE_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <chrono>
#include <fcppt/config/external_end.hpp>

namespace fruitlib
{
fcppt::string const
human_readable_time_difference(
	std::chrono::seconds const &);
}

#endif
