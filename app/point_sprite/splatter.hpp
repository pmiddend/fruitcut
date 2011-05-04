#ifndef FRUITCUT_APP_POINT_SPRITE_SPLATTER_HPP_INCLUDED
#define FRUITCUT_APP_POINT_SPRITE_SPLATTER_HPP_INCLUDED

#include "../../scenic/nodes/with_lifetime.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
{
namespace point_sprite
{
class splatter
:
	public scenic::nodes::with_lifetime
{
FCPPT_NONCOPYABLE(
	splatter);
public:
	explicit
	splatter();

	~splatter();
private:
	void
	update();

	void
	render();

	bool
	dead() const;
};
}
}
}

#endif
