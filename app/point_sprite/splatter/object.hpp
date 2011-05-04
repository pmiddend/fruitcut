#ifndef FRUITCUT_APP_POINT_SPRITE_SPLATTER_OBJECT_HPP_INCLUDED
#define FRUITCUT_APP_POINT_SPRITE_SPLATTER_OBJECT_HPP_INCLUDED

#include "parameters_fwd.hpp"
#include "../vector.hpp"
#include "../object.hpp"
#include "../base.hpp"
#include "../system.hpp"
#include "../color.hpp"
#include "gravity_callback.hpp"
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
	point_sprite::vector linear_velocity_;
	sge::time::timer life_timer_;
	sge::time::timer second_timer_;
	fruitcut::app::point_sprite::splatter::gravity_callback const gravity_callback_;
};
}
}
}
}

#endif
