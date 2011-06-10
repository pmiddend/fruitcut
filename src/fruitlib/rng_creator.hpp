#ifndef FRUITCUT_FRUITLIB_RNG_CREATOR_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_RNG_CREATOR_HPP_INCLUDED

#include "random_generator.hpp"

namespace fruitcut
{
namespace fruitlib
{
class rng_creator
{
public:
	typedef
	unsigned long
	value_type;

	explicit
	rng_creator(
		value_type);

	fruitlib::random_generator const
	create();
private:
	value_type rng_init_;
};
}
}

#endif
