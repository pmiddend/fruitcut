#ifndef FRUITCUT_FRUITLIB_TIME_FORMAT_DOMAIN_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_TIME_FORMAT_DOMAIN_HPP_INCLUDED

#include "formatter_fwd.hpp"
#include "grammar.hpp"
#include <boost/proto/proto.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace time_format
{
class domain
:
	boost::proto::domain<boost::proto::generator<formatter>,grammar>
{
};
}
}
}

#endif
