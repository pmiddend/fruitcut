#ifndef FRUITCUT_UNIFORM_RANDOM_HPP_INCLUDED
#define FRUITCUT_UNIFORM_RANDOM_HPP_INCLUDED

#include "random_generator.hpp"
#include <fcppt/random/uniform.hpp>
#include <fcppt/restrict_typedef_struct.hpp>

// See random_generator for why this is here
namespace fruitcut
{
template<typename T>
struct uniform_random
{
	typedef
	fcppt::random::uniform<T,random_generator>
	type;

	FCPPT_RESTRICT_TYPEDEF_STRUCT(
		uniform_random);
};
}

#endif
