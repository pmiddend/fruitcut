#ifndef FRUITAPP_POINT_SPRITE_SPLATTER_PARAMETERS_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_SPLATTER_PARAMETERS_HPP_INCLUDED

#include "../system.hpp"
#include "../color.hpp"
#include "position.hpp"
#include "linear_velocity.hpp"
#include "acceleration.hpp"
#include "size.hpp"
#include <sge/time/duration.hpp>
#include <sge/time/callback.hpp>
#include <sge/texture/part_ptr.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/nonassignable.hpp>

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
		fruitapp::point_sprite::system &,
		fruitapp::point_sprite::splatter::position const &,
		fruitapp::point_sprite::splatter::linear_velocity const &,
		fruitapp::point_sprite::splatter::acceleration const &,
		fruitapp::point_sprite::splatter::size const &,
		fruitapp::point_sprite::color const &,
		sge::texture::part_ptr,
		sge::time::duration const &,
		sge::time::callback const &);

	fruitapp::point_sprite::system &
	system() const;

	fruitapp::point_sprite::splatter::position::value_type const &
	position() const;

	fruitapp::point_sprite::splatter::linear_velocity::value_type const &
	linear_velocity() const;

	fruitapp::point_sprite::splatter::acceleration::value_type const &
	acceleration() const;

	fruitapp::point_sprite::splatter::size::value_type const &
	size() const;

	fruitapp::point_sprite::color const &
	color() const;

	sge::texture::part_ptr const
	texture() const;

	sge::time::duration const &
	life_time() const;

	sge::time::callback const &
	time_callback() const;

	~parameters();
private:
	fruitapp::point_sprite::system &system_;
	fruitapp::point_sprite::splatter::position::value_type const position_;
	fruitapp::point_sprite::splatter::linear_velocity::value_type const linear_velocity_;
	fruitapp::point_sprite::splatter::acceleration::value_type const acceleration_;
	fruitapp::point_sprite::splatter::size::value_type const size_;
	fruitapp::point_sprite::color const color_;
	sge::texture::part_ptr const texture_;
	sge::time::duration const life_time_;
	sge::time::callback const time_callback_;
};
}
}
}

#endif
