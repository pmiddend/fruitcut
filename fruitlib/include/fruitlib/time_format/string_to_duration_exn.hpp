#ifndef FRUITLIB_TIME_FORMAT_STRING_TO_DURATION_EXN_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_STRING_TO_DURATION_EXN_HPP_INCLUDED

#include <fruitlib/exception.hpp>
#include <fruitlib/time_format/string_to_duration.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/optional/to_exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/config/external_begin.hpp>
#include <string>
#include <fcppt/config/external_end.hpp>


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
	return
		fcppt::optional::to_exception(
			time_format::string_to_duration<TargetDuration>(
				input_string
			),
			[
				&input_string
			]{
				return
					fruitlib::exception(
						FCPPT_TEXT("\"")
						+
						input_string
						+
						FCPPT_TEXT("\" is not a valid time string")
					);
			}
		);
}
}
}

#endif
