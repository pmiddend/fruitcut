#ifndef FRUITLIB_TIME_FORMAT_MINUTES_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_MINUTES_HPP_INCLUDED

#include <fruitlib/time_format/tags/minutes.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/proto/proto.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace time_format
{
extern boost::proto::terminal<tags::minutes>::type const minutes;
}
}

#endif
