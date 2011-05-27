#ifndef FRUITCUT_APP_POINT_SPRITE_SPLATTER_OBJECT_HPP_INCLUDED
#define FRUITCUT_APP_POINT_SPRITE_SPLATTER_OBJECT_HPP_INCLUDED

#include "parameters_fwd.hpp"
#include "linear_velocity.hpp"
#include "acceleration.hpp"
#include "../vector.hpp"
#include "../object.hpp"
#include "../base.hpp"
#include "../system.hpp"
#include "../color.hpp"
#include <sge/time/timer.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
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
}

#endif
