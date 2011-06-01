#ifndef FRUITCUT_FRUITLIB_TIME_FORMAT_SECONDS_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_TIME_FORMAT_SECONDS_HPP_INCLUDED

#include "tags/seconds.hpp"
#include <boost/proto/proto.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace time_format
{
extern boost::proto::terminal<tags::seconds>::type const seconds;
}
}
}

#endif
