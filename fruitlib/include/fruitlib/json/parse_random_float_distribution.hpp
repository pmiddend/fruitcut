#ifndef FRUITLIB_JSON_PARSE_RANDOM_FLOAT_DISTRIBUTION_HPP_INCLUDED
#define FRUITLIB_JSON_PARSE_RANDOM_FLOAT_DISTRIBUTION_HPP_INCLUDED

#include <sge/parse/json/array.hpp>
#include <sge/parse/json/convert_from.hpp>
#include <fcppt/random/distribution/basic.hpp>
#include <fcppt/random/distribution/parameters/uniform_real.hpp>


namespace fruitlib
{
namespace json
{
template<typename T>
fcppt::random::distribution::basic<
	fcppt::random::distribution::parameters::uniform_real<T>
> const
parse_random_float_distribution(
	sge::parse::json::array const &a)
{
	return
		fcppt::random::distribution::basic<
			fcppt::random::distribution::parameters::uniform_real<T>
		>(
			typename fcppt::random::distribution::parameters::uniform_real<T>::min(
				sge::parse::json::convert_from<T>(
					a.elements[0])),
			typename fcppt::random::distribution::parameters::uniform_real<T>::sup(
				sge::parse::json::convert_from<T>(
					a.elements[1])));
}
}
}

#endif
