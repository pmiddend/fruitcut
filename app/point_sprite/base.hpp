#ifndef FRUITCUT_APP_POINT_SPRITE_BASE_HPP_INCLUDED
#define FRUITCUT_APP_POINT_SPRITE_BASE_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
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
}

#endif
