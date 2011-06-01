#ifndef FRUITCUT_FRUITLIB_TIME_FORMAT_GRAMMAR_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_TIME_FORMAT_GRAMMAR_HPP_INCLUDED

#include <boost/proto/proto.hpp>

namespace fruitcut
{
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
}

#endif
