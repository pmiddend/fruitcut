#include "rng_creator.hpp"

fruitcut::fruitlib::rng_creator::rng_creator(
	value_type const _rng_init)
:
	rng_init_(
		_rng_init)
{
}

fruitcut::fruitlib::random_generator const
fruitcut::fruitlib::rng_creator::create()
{
	return 
		fruitlib::random_generator(
			rng_init_++);
}
