#ifndef FRUITLIB_TIME_FORMAT_DURATION_TO_STRING_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_DURATION_TO_STRING_HPP_INCLUDED

#include <fruitlib/time_format/context.hpp>
#include <fruitlib/time_format/grammar.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/proto/proto.hpp>
#include <chrono>
#include <fcppt/config/external_end.hpp>


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
	std::chrono::duration<Rep,Period> const &d,
	ProtoExpression const &expression_tree)
{
	BOOST_MPL_ASSERT((boost::proto::matches<ProtoExpression, grammar>));
	return
		boost::proto::eval(
			expression_tree,
			context<std::chrono::duration<Rep,Period>,String>(
				d));
}
}
}

#endif
