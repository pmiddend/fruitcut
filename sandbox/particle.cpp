#include "particle.hpp"
#include <sge/image/color/init.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <fcppt/chrono/high_resolution_clock.hpp>

fruitcut::sandbox::particle::particle(
	sprite::parameters const &params,
	duration const &_life_time)
:
	sprite_(
		params.elements()),
	birth_(
		fcppt::chrono::high_resolution_clock::now()),
	life_time_(
		_life_time)
{
}

void
fruitcut::sandbox::particle::update()
{
	sprite_.color(
		(sge::image::color::init::red %= 1.0)
		(sge::image::color::init::green %= 1.0)
		(sge::image::color::init::blue %= 1.0)
		(sge::image::color::init::alpha %= 
			static_cast<double>(
				(fcppt::chrono::high_resolution_clock::now() - birth_).count())/
			static_cast<double>(
				life_time_.count())));
}

bool 
fruitcut::sandbox::particle::dead() const
{
	return 
		(fcppt::chrono::high_resolution_clock::now() - birth_).count() > life_time_.count();
}
