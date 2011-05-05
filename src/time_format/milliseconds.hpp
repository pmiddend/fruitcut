#ifndef FRUITCUT_TIME_FORMAT_MILLISECONDS_HPP_INCLUDED
#define FRUITCUT_TIME_FORMAT_MILLISECONDS_HPP_INCLUDED

#include "tags/milliseconds.hpp"
#include <boost/proto/proto.hpp>

namespace fruitcut
{
namespace time_format
{
extern boost::proto::terminal<tags::milliseconds>::type const milliseconds;
}
}

#endif
