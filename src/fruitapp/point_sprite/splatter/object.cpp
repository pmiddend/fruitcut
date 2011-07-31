#include "object.hpp"
#include "parameters.hpp"
#include "../parameters.hpp"
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>

fruitapp::point_sprite::splatter::object::object(
	parameters const &p)
:
	object_(
		fruitapp::point_sprite::parameters()
			.pos(
				fruitapp::point_sprite::object::vector(
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
			.system(
				&(p.system()))
			.color(
				p.color())
			.elements()),
	linear_velocity_(
		p.linear_velocity()),
	acceleration_(
		p.acceleration()),
	life_timer_(
		p.life_time(),
		sge::time::activation_state::active,
		p.time_callback()),
	second_timer_(
		sge::time::second(1),
		sge::time::activation_state::active,
		p.time_callback())
{
	
}

fruitapp::point_sprite::splatter::object::~object()
{
}

void
fruitapp::point_sprite::splatter::object::update()
{
	point_sprite::object::unit const time_delta = 
		static_cast<point_sprite::object::unit>(
			second_timer_.reset());

	object_.pos(
		object_.pos() + 
		time_delta * 
		point_sprite::object::vector(
			linear_velocity_[0],
			linear_velocity_[1]));
	object_.z(
		object_.z() + 
		time_delta * linear_velocity_[2]);
	linear_velocity_ += 
		time_delta * acceleration_;
}

bool
fruitapp::point_sprite::splatter::object::dead() const
{
	return life_timer_.expired();
}
