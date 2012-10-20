#ifndef FRUITLIB_UNIFORM_REAL_RANDOM_HPP_INCLUDED
#define FRUITLIB_UNIFORM_REAL_RANDOM_HPP_INCLUDED

#include <fruitlib/random_generator.hpp>
#include <fcppt/random/variate.hpp>
#include <fcppt/random/distribution/uniform_real.hpp>


// See random_generator for why this is here
namespace fruitlib
{
template<typename T>
struct uniform_real_random
{
	typedef
	fcppt::random::variate<
		fruitlib::random_generator,
		fcppt::random::distribution::uniform_real<
			T
		>
	>
	type;
};
}

#endif
