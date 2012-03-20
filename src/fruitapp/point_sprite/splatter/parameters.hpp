#ifndef FRUITAPP_POINT_SPRITE_SPLATTER_PARAMETERS_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_SPLATTER_PARAMETERS_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/point_sprite/color.hpp>
#include <fruitapp/point_sprite/connection.hpp>
#include <fruitapp/point_sprite/splatter/acceleration.hpp>
#include <fruitapp/point_sprite/splatter/linear_velocity.hpp>
#include <fruitapp/point_sprite/splatter/position.hpp>
#include <fruitapp/point_sprite/splatter/size.hpp>
#include <sge/texture/part_ptr.hpp>
#include <fcppt/nonassignable.hpp>
#include <fcppt/chrono/duration.hpp>


namespace fruitapp
{
namespace point_sprite
{
namespace splatter
{
class parameters
{
FCPPT_NONASSIGNABLE(
	parameters);
public:
	explicit
	parameters(
		point_sprite::connection &,
		point_sprite::splatter::position const &,
		point_sprite::splatter::linear_velocity const &,
		point_sprite::splatter::acceleration const &,
		point_sprite::splatter::size const &,
		point_sprite::color const &,
		sge::texture::part_ptr,
		fruitapp::ingame_clock::duration const &,
		fruitapp::ingame_clock const &);

	point_sprite::connection &
	connection() const;

	point_sprite::splatter::position::value_type const &
	position() const;

	point_sprite::splatter::linear_velocity::value_type const &
	linear_velocity() const;

	point_sprite::splatter::acceleration::value_type const &
	acceleration() const;

	point_sprite::splatter::size::value_type const &
	size() const;

	point_sprite::color const &
	color() const;

	sge::texture::part_ptr const
	texture() const;

	fruitapp::ingame_clock::duration const &
	life_time() const;

	fruitapp::ingame_clock const &
	clock() const;

	~parameters();
private:
	fruitapp::point_sprite::connection &connection_;
	fruitapp::point_sprite::splatter::position::value_type const position_;
	fruitapp::point_sprite::splatter::linear_velocity::value_type const linear_velocity_;
	fruitapp::point_sprite::splatter::acceleration::value_type const acceleration_;
	fruitapp::point_sprite::splatter::size::value_type const size_;
	fruitapp::point_sprite::color const color_;
	sge::texture::part_ptr const texture_;
	fruitapp::ingame_clock::duration const life_time_;
	fruitapp::ingame_clock const &clock_;
};
}
}
}

#endif
