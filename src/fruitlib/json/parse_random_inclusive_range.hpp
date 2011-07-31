#ifndef FRUITLIB_JSON_PARSE_RANDOM_INCLUSIVE_RANGE_HPP_INCLUDED
#define FRUITLIB_JSON_PARSE_RANDOM_INCLUSIVE_RANGE_HPP_INCLUDED

#include "convert_from.hpp"
#include <fcppt/random/make_inclusive_range.hpp>
#include <fcppt/random/inclusive_range.hpp>
#include <sge/parse/json/array.hpp>

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
			json::convert_from<T>(
				a.elements[0]),
			json::convert_from<T>(
				a.elements[1]));
}
}
}

#endif
