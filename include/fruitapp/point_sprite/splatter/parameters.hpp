#ifndef FRUITAPP_POINT_SPRITE_SPLATTER_PARAMETERS_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_SPLATTER_PARAMETERS_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/point_sprite/color.hpp>
#include <fruitapp/point_sprite/connection.hpp>
#include <fruitapp/point_sprite/splatter/acceleration.hpp>
#include <fruitapp/point_sprite/splatter/linear_velocity.hpp>
#include <fruitapp/point_sprite/splatter/position.hpp>
#include <fruitapp/point_sprite/splatter/size.hpp>
#include <sge/texture/const_optional_part_ref.hpp>
#include <fcppt/nonassignable.hpp>
#include <sge/renderer/target/base_fwd.hpp>
#include <fcppt/preprocessor/pure.hpp>
#include <fcppt/preprocessor/warn_unused_result.hpp>
#include <sge/camera/base_fwd.hpp>


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
		sge::camera::base const &,
		sge::renderer::target::base const &,
		fruitapp::point_sprite::connection &,
		fruitapp::point_sprite::splatter::position const &,
		fruitapp::point_sprite::splatter::linear_velocity const &,
		fruitapp::point_sprite::splatter::acceleration const &,
		fruitapp::point_sprite::splatter::size const &,
		fruitapp::point_sprite::color const &,
		sge::texture::const_optional_part_ref const &,
		fruitapp::ingame_clock::duration const &,
		fruitapp::ingame_clock const &);

	sge::camera::base const &
	camera() const
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

	sge::renderer::target::base const &
	target() const
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

	sge::texture::const_optional_part_ref const &
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
	sge::camera::base const &camera_;
	sge::renderer::target::base const &target_;
	fruitapp::point_sprite::connection &connection_;
	fruitapp::point_sprite::splatter::position const position_;
	fruitapp::point_sprite::splatter::linear_velocity const linear_velocity_;
	fruitapp::point_sprite::splatter::acceleration const acceleration_;
	fruitapp::point_sprite::splatter::size const size_;
	fruitapp::point_sprite::color const color_;
	sge::texture::const_optional_part_ref const texture_;
	fruitapp::ingame_clock::duration const life_time_;
	fruitapp::ingame_clock const &clock_;
};
}
}
}

#endif
