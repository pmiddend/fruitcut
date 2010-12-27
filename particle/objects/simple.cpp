#include "simple.hpp"
#include "../point_sprite/choices.hpp"
#include "../sprite/choices.hpp"
#include <sge/time/second_f.hpp>
#include <sge/time/funit.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <mizuiro/color/operators/add.hpp>
#include <mizuiro/color/operators/scalar_multiply.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>

template<typename Choices>
fruitcut::particle::objects::simple<Choices>::simple(
	sprite_parameters const &params,
	typename color_animation::value_sequence const &_animation,
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

template<typename Choices>
void
fruitcut::particle::objects::simple<Choices>::update()
{
	sge::renderer::scalar const delta = 
		static_cast<sge::renderer::scalar>(
			seconds_timer_.reset());
	position_ += 
		delta * velocity_;
	velocity_ += 
		delta * acceleration_;
	sprite_.pos(
		fcppt::math::vector::structure_cast<typename sprite_object::point>(
			position_));
	animation_.update();
	sprite_.color(
		animation_.current_value());
}

template<typename Choices>
bool 
fruitcut::particle::objects::simple<Choices>::dead() const
{
	return animation_.finished();
}

template<typename Choices>
void
fruitcut::particle::objects::simple<Choices>::velocity(
	sge::renderer::vector2 const &_velocity)
{
	velocity_ = _velocity;
}

template<typename Choices>
void
fruitcut::particle::objects::simple<Choices>::acceleration(
	sge::renderer::vector2 const &_acceleration)
{
	acceleration_ = _acceleration;
}

template<typename Choices>
fruitcut::particle::objects::simple<Choices>::~simple()
{
}

template class fruitcut::particle::objects::simple<fruitcut::particle::sprite::choices>;
template class fruitcut::particle::objects::simple<fruitcut::particle::point_sprite::choices>;
