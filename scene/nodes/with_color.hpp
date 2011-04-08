#ifndef FRUITCUT_SCENE_NODES_WITH_COLOR_HPP_INCLUDED
#define FRUITCUT_SCENE_NODES_WITH_COLOR_HPP_INCLUDED

#include "base.hpp"
#include "../color.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace scene
{
namespace nodes
{
class with_color
:
	public virtual base
{
FCPPT_NONCOPYABLE(
	with_color);
public:
	virtual
	fruitcut::scene::color const
	color() const = 0;

	virtual void
	color(
		fruitcut::scene::color const &) = 0;

	~with_color();
protected:
	with_color();
};
}
}
}

#endif
