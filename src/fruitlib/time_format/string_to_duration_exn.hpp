#ifndef FRUITLIB_TIME_FORMAT_STRING_TO_DURATION_EXN_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_STRING_TO_DURATION_EXN_HPP_INCLUDED

#include "string_to_duration.hpp"
#include "../exception.hpp"
#include <fcppt/optional.hpp>
#include <fcppt/text.hpp>
#include <string>

namespace fruitlib
{
namespace time_format
{
template
<
	typename TargetDuration,
	typename Char,
	typename Traits
>
TargetDuration const
string_to_duration_exn(
	std::basic_string<Char,Traits> const &input_string)
{
	fcppt::optional<TargetDuration> result = 
		time_format::string_to_duration<TargetDuration>(
			input_string);

	if(!result)
		throw fruitlib::exception(FCPPT_TEXT("\"")+input_string+FCPPT_TEXT("\" is not a valid time string"));

	return *result;
}
}
}

#endif
