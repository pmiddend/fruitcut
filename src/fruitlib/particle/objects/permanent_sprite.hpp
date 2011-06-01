#ifndef FRUITCUT_FRUITLIB_PARTICLE_OBJECTS_PERMANENT_SPRITE_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PARTICLE_OBJECTS_PERMANENT_SPRITE_HPP_INCLUDED

#include "base.hpp"
#include "../sprite/parameters.hpp"
#include "../sprite/object.hpp"
#include <sge/sprite/object_impl.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace particle
{
namespace objects
{
// NOTE: Could this be the base for "simple"?
class permanent_sprite
:
	public base
{
public:
	explicit
	permanent_sprite(
		sprite::parameters const &);

	void
	update();

	bool
	dead() const;

	sprite::object &
	sprite();

	sprite::object const &
	sprite() const;
private:
	sprite::object sprite_;
};
}
}
}
}

#endif
