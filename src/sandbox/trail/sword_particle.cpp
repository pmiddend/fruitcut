#include "sword_particle.hpp"
#include <sge/image/color/init.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <fcppt/chrono/high_resolution_clock.hpp>
#include <fcppt/math/dim/static.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

fruitcut::sandbox::sword_particle::sword_particle(
	particle::sprite::parameters const &params,
	duration const &_life_time)
:
	sprite_(
		params.elements()),
	initial_size_(
		sprite_.size()),
	birth_(
		fcppt::chrono::high_resolution_clock::now()),
	life_time_(
		_life_time)
{
}

void
fruitcut::sandbox::sword_particle::update()
{
	double const life_percentage = 
		1.0 - 
		static_cast<double>(
			(fcppt::chrono::high_resolution_clock::now() - birth_).count())/ 
		static_cast<double>(
			life_time_.count());

	particle::sprite::object::vector const center = 
		sprite_.pos() + sprite_.size()/2;

	sprite_.size(
		particle::sprite::object::dim(
			sprite_.size().w(),
			static_cast<particle::sprite::object::unit>(
				static_cast<double>(initial_size_.h()) * life_percentage)));
	/*
	sprite_.size(
		fcppt::math::dim::structure_cast<particle::sprite::object::dim>(
			life_percentage * 
			fcppt::math::dim::structure_cast<fcppt::math::dim::static_<double,2>::type>(
				initial_size_)));
	*/

	sprite_.pos(
		center - sprite_.size()/2);

	/*
	sprite_.color(
		(sge::image::color::init::red %= 1.0)
		(sge::image::color::init::green %= 1.0)
		(sge::image::color::init::blue %= 1.0)
		(sge::image::color::init::alpha %= 
			life_percentage));
	*/
}

bool 
fruitcut::sandbox::sword_particle::dead() const
{
	return 
		(fcppt::chrono::high_resolution_clock::now() - birth_).count() > life_time_.count();
}

fruitcut::sandbox::sword_particle::~sword_particle()
{
}
