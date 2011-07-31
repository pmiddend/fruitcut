#ifndef FRUITLIB_TIME_FORMAT_GRAMMAR_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_GRAMMAR_HPP_INCLUDED

#include <boost/proto/proto.hpp>

namespace fruitlib
{
namespace time_format
{
class grammar
:
	public 
		boost::proto::or_
		<
			boost::proto::plus<grammar,grammar>,
			boost::proto::terminal<boost::proto::_>
		>
{
public:
};
}
}

#endif
