#ifndef FRUITLIB_UNIFORM_INT_RANDOM_HPP_INCLUDED
#define FRUITLIB_UNIFORM_INT_RANDOM_HPP_INCLUDED

#include <fruitlib/random_generator.hpp>
#include <fcppt/random/variate.hpp>
#include <fcppt/random/distribution/basic.hpp>
#include <fcppt/random/distribution/parameters/uniform_int.hpp>


// See random_generator for why this is here
namespace fruitlib
{
template<typename T>
struct uniform_int_random
{
	typedef
	fcppt::random::variate<
		fruitlib::random_generator,
		fcppt::random::distribution::basic<
			fcppt::random::distribution::parameters::uniform_int<
				T
			>
		>
	>
	type;
};
}

#endif
