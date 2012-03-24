#include <fruitapp/point_sprite/parameters.hpp>
#include <fruitapp/point_sprite/splatter/object.hpp>
#include <fruitapp/point_sprite/splatter/parameters.hpp>
#include <sge/sprite/intrusive/connection.hpp>
#include <sge/timer/elapsed_fractional_and_reset.hpp>
#include <sge/timer/parameters.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/duration.hpp>
#include <fcppt/config/external_end.hpp>


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
			.connection(
				p.connection())
			.color(
				p.color())
			.elements()),
	linear_velocity_(
		p.linear_velocity()),
	acceleration_(
		p.acceleration()),
	life_timer_(
		fruitapp::ingame_timer::parameters(
			p.clock(),
			p.life_time())),
	second_timer_(
		fruitapp::ingame_timer::parameters(
			p.clock(),
			boost::chrono::seconds(
				1)))
{

}

fruitapp::point_sprite::splatter::object::~object()
{
}

void
fruitapp::point_sprite::splatter::object::update()
{
	point_sprite::object::unit const time_delta =
		sge::timer::elapsed_fractional_and_reset<point_sprite::object::unit>(
			second_timer_);

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
