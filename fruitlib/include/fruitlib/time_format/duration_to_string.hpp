#ifndef FRUITLIB_TIME_FORMAT_DURATION_TO_STRING_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_DURATION_TO_STRING_HPP_INCLUDED

#include <fruitlib/time_format/context.hpp>
#include <fruitlib/time_format/grammar.hpp>
#include <fcppt/preprocessor/disable_gcc_warning.hpp>
#include <fcppt/preprocessor/pop_warning.hpp>
#include <fcppt/preprocessor/push_warning.hpp>
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
String
duration_to_string(
	std::chrono::duration<Rep,Period> const &d,
	ProtoExpression const &expression_tree)
{
	FCPPT_PP_PUSH_WARNING
	FCPPT_PP_DISABLE_GCC_WARNING(-Wold-style-cast)
	BOOST_MPL_ASSERT((boost::proto::matches<ProtoExpression, grammar>));
	FCPPT_PP_POP_WARNING

	return
		boost::proto::eval(
			expression_tree,
			context<std::chrono::duration<Rep,Period>,String>(
				d));
}
}
}

#endif
