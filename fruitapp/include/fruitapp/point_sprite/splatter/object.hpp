#ifndef FRUITAPP_POINT_SPRITE_SPLATTER_OBJECT_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_SPLATTER_OBJECT_HPP_INCLUDED

#include <fruitapp/ingame_timer.hpp>
#include <fruitapp/point_sprite/base.hpp>
#include <fruitapp/point_sprite/object.hpp>
#include <fruitapp/point_sprite/splatter/acceleration.hpp>
#include <fruitapp/point_sprite/splatter/linear_velocity.hpp>
#include <fruitapp/point_sprite/splatter/parameters_fwd.hpp>
#include <fruitapp/point_sprite/splatter/position.hpp>
#include <fruitapp/projection_manager/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
namespace point_sprite
{
namespace splatter
{
class object
:
	public fruitapp::point_sprite::base
{
FCPPT_NONCOPYABLE(
	object);
public:
	explicit
	object(
		fruitapp::point_sprite::splatter::parameters const &);

	~object();
private:
	void
	update();

	bool
	dead() const;

	fruitapp::projection_manager::object const &projection_manager_;
	fruitapp::point_sprite::splatter::position position_;
	fruitapp::point_sprite::splatter::linear_velocity linear_velocity_;
	fruitapp::point_sprite::splatter::acceleration acceleration_;
	fruitapp::point_sprite::object object_;
	fruitapp::ingame_timer life_timer_;
	fruitapp::ingame_timer second_timer_;
};
}
}
}

#endif
