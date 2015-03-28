#ifndef FRUITLIB_HUMAN_READABLE_TIME_DIFFERENCE_HPP_INCLUDED
#define FRUITLIB_HUMAN_READABLE_TIME_DIFFERENCE_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fcppt/string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <chrono>
#include <fcppt/config/external_end.hpp>

namespace fruitlib
{

FRUITLIB_DETAIL_SYMBOL
fcppt::string
human_readable_time_difference(
	std::chrono::seconds const &);
}

#endif
