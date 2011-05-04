#include "object.hpp"
#include "parameters.hpp"
#include "../parameters.hpp"
#include <sge/time/timer.hpp>

fruitcut::app::point_sprite::splatter::object::object(
	parameters const &p)
:
	object_(
		fruitcut::app::point_sprite::parameters()
			.pos(
				fruitcut::app::point_sprite::object::vector(
					p.position()[0],
					p.position()[1]))
			.depth(
				p.position()[2])
			.point_size(
				p.size())
			.texture(
				p.texture())
			.order(
				0)
			.color(
				p.color())
			.elements()),
	linear_velocity_(
		p.linear_velocity()),
	life_timer_(
		p.life_time(),
		sge::time::activation_state::active,
		p.time_callback()),
	second_timer_(
		sge::time::second(1),
		sge::time::activation_state::active,
		p.time_callback()),
	gravity_callback_(
		p.gravity_callback())
{
	
}

fruitcut::app::point_sprite::splatter::object::~object()
{
}

void
fruitcut::app::point_sprite::splatter::object::update()
{
	object_.pos(
		object_.pos() + 
		static_cast<point_sprite::object::unit>(
			second_timer_.reset()) * 
		point_sprite::object::vector(
			linear_velocity_[0],
			linear_velocity_[1]));
	object_.z(
		object_.z() + 
		linear_velocity_[2]);
	linear_velocity_ += 
		gravity_callback_();
}

bool
fruitcut::app::point_sprite::splatter::object::dead() const
{
	return life_timer_.expired();
}
