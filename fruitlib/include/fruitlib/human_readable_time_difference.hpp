#ifndef FRUITLIB_HUMAN_READABLE_TIME_DIFFERENCE_HPP_INCLUDED
#define FRUITLIB_HUMAN_READABLE_TIME_DIFFERENCE_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono.hpp>
#include <fcppt/config/external_end.hpp>

namespace fruitlib
{
fcppt::string const
human_readable_time_difference(
	boost::chrono::seconds const &);
}

#endif
