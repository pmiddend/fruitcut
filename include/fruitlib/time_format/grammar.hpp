#ifndef FRUITLIB_TIME_FORMAT_GRAMMAR_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_GRAMMAR_HPP_INCLUDED

#include <fcppt/preprocessor/disable_gcc_warning.hpp>
#include <fcppt/preprocessor/pop_warning.hpp>
#include <fcppt/preprocessor/push_warning.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/proto/proto.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace time_format
{
FCPPT_PP_PUSH_WARNING
FCPPT_PP_DISABLE_GCC_WARNING(-Weffc++)

struct grammar
:
	public
		boost::proto::or_
		<
			boost::proto::plus<grammar,grammar>,
			boost::proto::terminal<boost::proto::_>
		>
{
};

FCPPT_PP_POP_WARNING
}
}

#endif
