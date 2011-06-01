#ifndef FRUITCUT_FRUITLIB_JSON_PARSE_RANDOM_INCLUSIVE_RANGE_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_JSON_PARSE_RANDOM_INCLUSIVE_RANGE_HPP_INCLUDED

#include "convert.hpp"
#include <fcppt/random/make_inclusive_range.hpp>
#include <fcppt/random/inclusive_range.hpp>
#include <sge/parse/json/array.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace json
{
template<typename T>
fcppt::random::inclusive_range<T> const
parse_random_inclusive_range(
	sge::parse::json::array const &a)
{
	return 
		fcppt::random::make_inclusive_range(
			json::convert<T>(
				a.elements[0]),
			json::convert<T>(
				a.elements[1]));
}
}
}
}

#endif
