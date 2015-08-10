#ifndef FRUITAPP_POINT_SPRITE_SPLATTER_PARAMETERS_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_SPLATTER_PARAMETERS_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/point_sprite/color.hpp>
#include <fruitapp/point_sprite/connection.hpp>
#include <fruitapp/point_sprite/splatter/acceleration.hpp>
#include <fruitapp/point_sprite/splatter/linear_velocity.hpp>
#include <fruitapp/point_sprite/splatter/position.hpp>
#include <fruitapp/point_sprite/splatter/size.hpp>
#include <fruitapp/projection_manager/object_fwd.hpp>
#include <sge/texture/const_part_ref.hpp>
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
	parameters(
		fruitapp::projection_manager::object const &,
		fruitapp::point_sprite::connection &,
		fruitapp::point_sprite::splatter::position const &,
		fruitapp::point_sprite::splatter::linear_velocity const &,
		fruitapp::point_sprite::splatter::acceleration const &,
		fruitapp::point_sprite::splatter::size const &,
		fruitapp::point_sprite::color const &,
		sge::texture::const_part_ref const &,
		fruitapp::ingame_clock::duration const &,
		fruitapp::ingame_clock const &);

	fruitapp::projection_manager::object const &
	projection_manager() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	fruitapp::point_sprite::connection &
	connection() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	fruitapp::point_sprite::splatter::position const &
	position() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	fruitapp::point_sprite::splatter::linear_velocity const &
	linear_velocity() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	fruitapp::point_sprite::splatter::acceleration const &
	acceleration() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	fruitapp::point_sprite::splatter::size const &
	size() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	fruitapp::point_sprite::color const &
	color() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	sge::texture::const_part_ref const &
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
	fruitapp::projection_manager::object const &projection_manager_;
	fruitapp::point_sprite::connection &connection_;
	fruitapp::point_sprite::splatter::position const position_;
	fruitapp::point_sprite::splatter::linear_velocity const linear_velocity_;
	fruitapp::point_sprite::splatter::acceleration const acceleration_;
	fruitapp::point_sprite::splatter::size const size_;
	fruitapp::point_sprite::color const color_;
	sge::texture::const_part_ref const texture_;
	fruitapp::ingame_clock::duration const life_time_;
	fruitapp::ingame_clock const &clock_;
};
}
}
}

#endif
