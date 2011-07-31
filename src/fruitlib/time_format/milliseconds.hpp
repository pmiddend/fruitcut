#ifndef FRUITLIB_TIME_FORMAT_MILLISECONDS_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_MILLISECONDS_HPP_INCLUDED

#include <fruitlib/time_format/tags/milliseconds.hpp>
#include <boost/proto/proto.hpp>

namespace fruitlib
{
namespace time_format
{
extern boost::proto::terminal<tags::milliseconds>::type const milliseconds;
}
}

#endif
