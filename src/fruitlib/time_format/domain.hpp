#ifndef FRUITLIB_TIME_FORMAT_DOMAIN_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_DOMAIN_HPP_INCLUDED

#include "formatter_fwd.hpp"
#include "grammar.hpp"
#include <boost/proto/proto.hpp>

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

#endif
