#ifndef FRUITCUT_FRUITLIB_UNIFORM_RANDOM_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_UNIFORM_RANDOM_HPP_INCLUDED

#include "random_generator.hpp"
#include <fcppt/random/uniform.hpp>

// See random_generator for why this is here
namespace fruitcut
{
namespace fruitlib
{
template<typename T>
struct uniform_random
{
	typedef
	fcppt::random::uniform<T,fruitlib::random_generator &>
	type;
};
}
}

#endif
