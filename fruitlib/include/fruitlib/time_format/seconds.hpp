#ifndef FRUITLIB_TIME_FORMAT_SECONDS_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_SECONDS_HPP_INCLUDED

#include <fruitlib/time_format/tags/seconds.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/proto/proto.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace time_format
{
extern boost::proto::terminal<tags::seconds>::type const seconds;
}
}

#endif
