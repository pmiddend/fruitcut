#ifndef FRUITCUT_RANDOM_GENERATOR_HPP_INCLUDED
#define FRUITCUT_RANDOM_GENERATOR_HPP_INCLUDED

#include <fcppt/random/default_generator.hpp>

// This is a bit misplaced here, it should be in app/, but audio/
// needs it and I didn't want to put that in app, so...
namespace fruitcut
{
typedef
fcppt::random::default_generator
random_generator;
}

#endif
