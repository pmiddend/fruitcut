#ifndef FRUITLIB_TIME_FORMAT_DURATION_TO_STRING_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_DURATION_TO_STRING_HPP_INCLUDED

#include "context.hpp"
#include "grammar.hpp"
#include <fcppt/chrono/duration.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/proto/proto.hpp>

namespace fruitlib
{
namespace time_format
{
template
<
	typename String,
	typename Rep,
	typename Period,
	typename ProtoExpression
>
String const
duration_to_string(
	fcppt::chrono::duration<Rep,Period> const &d,
	ProtoExpression const &expression_tree)
{
	BOOST_MPL_ASSERT((boost::proto::matches<ProtoExpression, grammar>));
	return 
		boost::proto::eval(
			expression_tree,
			context<fcppt::chrono::duration<Rep,Period>,String>(
				d));
}
}
}

#endif
