#ifndef FRUITLIB_TIME_FORMAT_DOMAIN_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_DOMAIN_HPP_INCLUDED

#include <fruitlib/time_format/formatter_fwd.hpp>
#include <fruitlib/time_format/grammar.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/proto/proto.hpp>
#include <fcppt/config/external_end.hpp>


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
