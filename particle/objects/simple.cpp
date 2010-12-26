#include "simple.hpp"
#include "../../sprite/object.hpp"
#include <sge/time/second_f.hpp>
#include <sge/time/funit.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/sprite/parameters.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>

fruitcut::particle::objects::simple::simple(
	sprite::parameters const &params,
	animation::value_sequence const &_animation,
	sge::renderer::vector2 const &_velocity,
	sge::renderer::vector2 const &_acceleration)
:
	sprite_(
		params.elements()),
	animation_(
		_animation),
	seconds_timer_(
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
fruitcut::particle::objects::simple::update()
{
	sge::renderer::scalar const delta = 
		static_cast<sge::renderer::scalar>(
			seconds_timer_.reset());
	position_ += 
		delta * velocity_;
	velocity_ += 
		delta * acceleration_;
	sprite_.pos(
		fcppt::math::vector::structure_cast<sprite::object::point>(
			position_));
	animation_.update();
	sprite_.color(
		animation_.current_value());
}

bool 
fruitcut::particle::objects::simple::dead() const
{
	return animation_.finished();
}

void
fruitcut::particle::objects::simple::acceleration(
	sge::renderer::vector2 const &_acceleration)
{
	acceleration_ = _acceleration;
}

fruitcut::particle::objects::simple::~simple()
{
}
