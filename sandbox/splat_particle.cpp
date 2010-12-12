#include "splat_particle.hpp"
#include <sge/image/color/init.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/time/second_f.hpp>
#include <sge/time/funit.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/chrono/high_resolution_clock.hpp>
#include <fcppt/math/dim/static.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>

fruitcut::sandbox::splat_particle::splat_particle(
	sprite::parameters const &params,
	sge::time::duration const &_life_time,
	sge::renderer::vector2 const &_velocity,
	sge::renderer::vector2 const &_acceleration)
:
	sprite_(
		params.elements()),
	timer_(
		_life_time),
	second_timer_(
		sge::time::second_f(
			static_cast<sge::time::funit>(
				1))),
	position_(
		fcppt::math::vector::structure_cast<sge::renderer::vector2>(
			sprite_.pos())),
	velocity_(
		_velocity),
	acceleration_(
		_acceleration)
{
}

void
fruitcut::sandbox::splat_particle::update()
{
	sge::renderer::scalar const delta = 
		static_cast<sge::renderer::scalar>(
			second_timer_.reset());
	position_ += 
		delta * velocity_;
	velocity_ += 
		delta * acceleration_;
}

bool 
fruitcut::sandbox::splat_particle::dead() const
{
	return timer_.expired();
}

fruitcut::sandbox::splat_particle::~splat_particle()
{
}
