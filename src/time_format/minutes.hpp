#ifndef FRUITCUT_TIME_FORMAT_MINUTES_HPP_INCLUDED
#define FRUITCUT_TIME_FORMAT_MINUTES_HPP_INCLUDED

#include "tags/minutes.hpp"
#include <boost/proto/proto.hpp>

namespace fruitcut
{
namespace time_format
{
extern boost::proto::terminal<tags::minutes>::type const minutes;
}
}

#endif
