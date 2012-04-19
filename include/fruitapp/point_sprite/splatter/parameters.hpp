#ifndef FRUITAPP_POINT_SPRITE_SPLATTER_PARAMETERS_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_SPLATTER_PARAMETERS_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/point_sprite/color.hpp>
#include <fruitapp/point_sprite/connection.hpp>
#include <fruitapp/point_sprite/splatter/acceleration.hpp>
#include <fruitapp/point_sprite/splatter/linear_velocity.hpp>
#include <fruitapp/point_sprite/splatter/position.hpp>
#include <fruitapp/point_sprite/splatter/size.hpp>
#include <sge/texture/part_shared_ptr.hpp>
#include <fcppt/nonassignable.hpp>
#include <fcppt/preprocessor/pure.hpp>
#include <fcppt/preprocessor/warn_unused_result.hpp>


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
		sge::texture::part_shared_ptr,
		fruitapp::ingame_clock::duration const &,
		fruitapp::ingame_clock const &);

	point_sprite::connection &
	connection() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	point_sprite::splatter::position::value_type const &
	position() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	point_sprite::splatter::linear_velocity::value_type const &
	linear_velocity() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	point_sprite::splatter::acceleration::value_type const &
	acceleration() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	point_sprite::splatter::size::value_type const &
	size() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	point_sprite::color const &
	color() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	sge::texture::part_shared_ptr const
	texture() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	fruitapp::ingame_clock::duration const &
	life_time() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	fruitapp::ingame_clock const &
	clock() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	~parameters();
private:
	fruitapp::point_sprite::connection &connection_;
	fruitapp::point_sprite::splatter::position::value_type const position_;
	fruitapp::point_sprite::splatter::linear_velocity::value_type const linear_velocity_;
	fruitapp::point_sprite::splatter::acceleration::value_type const acceleration_;
	fruitapp::point_sprite::splatter::size::value_type const size_;
	fruitapp::point_sprite::color const color_;
	sge::texture::part_shared_ptr const texture_;
	fruitapp::ingame_clock::duration const life_time_;
	fruitapp::ingame_clock const &clock_;
};
}
}
}

#endif
