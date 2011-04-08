#ifndef FRUITCUT_SCENE_NODES_WITH_SCALE_HPP_INCLUDED
#define FRUITCUT_SCENE_NODES_WITH_SCALE_HPP_INCLUDED

#include "base.hpp"
#include "../scale.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace scene
{
namespace nodes
{
class with_scale
:
	public virtual base
{
FCPPT_NONCOPYABLE(
	with_scale);
public:
	virtual
	fruitcut::scene::scale
	scale() const = 0;

	virtual void
	scale(
		fruitcut::scene::scale) = 0;
	~with_scale();
protected:
	with_scale();
};
}
}
}

#endif
