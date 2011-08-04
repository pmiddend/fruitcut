#ifndef FRUITLIB_TIME_FORMAT_FIND_AND_CONVERT_DURATION_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_FIND_AND_CONVERT_DURATION_HPP_INCLUDED

#include <fruitlib/json/path_fwd.hpp>
#include <fruitlib/json/find_and_convert_member.hpp>
#include <fruitlib/time_format/string_to_duration_exn.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/string.hpp>

namespace fruitlib
{
namespace time_format
{
template<typename Duration>
Duration const
find_and_convert_duration(
	sge::parse::json::object const &o,
	fruitlib::json::path const &p)
{
	return 
		fruitlib::time_format::string_to_duration_exn<Duration>(
			fruitlib::json::find_and_convert_member<fcppt::string>(
				o,
				p));
}
}
}

#endif
