#ifndef FRUITLIB_JSON_PARSE_RANDOM_INT_DISTRIBUTION_HPP_INCLUDED
#define FRUITLIB_JSON_PARSE_RANDOM_INT_DISTRIBUTION_HPP_INCLUDED

#include <sge/parse/json/array.hpp>
#include <sge/parse/json/convert_from.hpp>
#include <fcppt/random/distribution/uniform_int_decl.hpp>

namespace fruitlib
{
namespace json
{
template<typename T>
fcppt::random::distribution::uniform_int<T> const
parse_random_int_distribution(
	sge::parse::json::array const &a)
{
	return
		fcppt::random::distribution::uniform_int<T>(
			typename fcppt::random::distribution::uniform_int<T>::min(
				sge::parse::json::convert_from<T>(
					a.elements[0])),
			typename fcppt::random::distribution::uniform_int<T>::max(
				sge::parse::json::convert_from<T>(
					a.elements[1])));
}
}
}

#endif
