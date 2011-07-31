#ifndef FRUITAPP_POINT_SPRITE_SPLATTER_OBJECT_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_SPLATTER_OBJECT_HPP_INCLUDED

#include "parameters_fwd.hpp"
#include "linear_velocity.hpp"
#include "acceleration.hpp"
#include <fruitapp/point_sprite/vector.hpp>
#include <fruitapp/point_sprite/object.hpp>
#include <fruitapp/point_sprite/base.hpp>
#include <fruitapp/point_sprite/system.hpp>
#include <fruitapp/point_sprite/color.hpp>
#include <sge/time/timer.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitapp
{
namespace point_sprite
{
namespace splatter
{
class object
:
	public point_sprite::base
{
FCPPT_NONCOPYABLE(
	object);
public:
	explicit
	object(
		parameters const &);

	~object();
private:
	void
	update();

	bool
	dead() const;

	point_sprite::object object_;
	splatter::linear_velocity::value_type linear_velocity_;
	splatter::acceleration::value_type acceleration_;
	sge::time::timer life_timer_;
	sge::time::timer second_timer_;
};
}
}
}

#endif
