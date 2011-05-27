#ifndef FRUITCUT_APP_POINT_SPRITE_SPLATTER_PARAMETERS_HPP_INCLUDED
#define FRUITCUT_APP_POINT_SPRITE_SPLATTER_PARAMETERS_HPP_INCLUDED

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

namespace fruitcut
{
namespace app
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
		fruitcut::app::point_sprite::system &,
		fruitcut::app::point_sprite::splatter::position const &,
		fruitcut::app::point_sprite::splatter::linear_velocity const &,
		fruitcut::app::point_sprite::splatter::acceleration const &,
		fruitcut::app::point_sprite::splatter::size const &,
		fruitcut::app::point_sprite::color const &,
		sge::texture::part_ptr,
		sge::time::duration const &,
		sge::time::callback const &);

	fruitcut::app::point_sprite::system &
	system() const;

	fruitcut::app::point_sprite::splatter::position::value_type const &
	position() const;

	fruitcut::app::point_sprite::splatter::linear_velocity::value_type const &
	linear_velocity() const;

	fruitcut::app::point_sprite::splatter::acceleration::value_type const &
	acceleration() const;

	fruitcut::app::point_sprite::splatter::size::value_type const &
	size() const;

	fruitcut::app::point_sprite::color const &
	color() const;

	sge::texture::part_ptr const
	texture() const;

	sge::time::duration const &
	life_time() const;

	sge::time::callback const &
	time_callback() const;

	~parameters();
private:
	fruitcut::app::point_sprite::system &system_;
	fruitcut::app::point_sprite::splatter::position::value_type const position_;
	fruitcut::app::point_sprite::splatter::linear_velocity::value_type const linear_velocity_;
	fruitcut::app::point_sprite::splatter::acceleration::value_type const acceleration_;
	fruitcut::app::point_sprite::splatter::size::value_type const size_;
	fruitcut::app::point_sprite::color const color_;
	sge::texture::part_ptr const texture_;
	sge::time::duration const life_time_;
	sge::time::callback const time_callback_;
};
}
}
}
}

#endif
