#include "create_rng.hpp"
#include <fcppt/chrono/high_resolution_clock.hpp>
#include <fcppt/chrono/time_point.hpp>

fruitcut::app::random_generator const
fruitcut::app::create_rng()
{
	static unsigned long init = 
		static_cast<unsigned long>(
			fcppt::chrono::high_resolution_clock::now().time_since_epoch().count());
	return 
		random_generator(
			init++);
}
