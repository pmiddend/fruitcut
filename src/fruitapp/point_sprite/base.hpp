#ifndef FRUITAPP_POINT_SPRITE_BASE_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_BASE_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>

namespace fruitapp
{
namespace point_sprite
{
class base
{
FCPPT_NONCOPYABLE(
	base);
public:
	virtual void
	update() = 0;

	virtual bool
	dead() const = 0;

	virtual ~base();
protected:
	base();
};
}
}

#endif
