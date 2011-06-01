#ifndef FRUITCUT_FRUITLIB_TIME_FORMAT_MILLISECONDS_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_TIME_FORMAT_MILLISECONDS_HPP_INCLUDED

#include "tags/milliseconds.hpp"
#include <boost/proto/proto.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace time_format
{
extern boost::proto::terminal<tags::milliseconds>::type const milliseconds;
}
}
}

#endif
