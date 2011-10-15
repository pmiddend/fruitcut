#ifndef FRUITLIB_UNIFORM_RANDOM_HPP_INCLUDED
#define FRUITLIB_UNIFORM_RANDOM_HPP_INCLUDED

#include <fruitlib/random_generator.hpp>
#include <fcppt/random/uniform.hpp>

// See random_generator for why this is here
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

#endif
