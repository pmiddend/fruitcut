#ifndef FRUITCUT_FRUITLIB_SCENIC_NODES_WITH_COLOR_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_NODES_WITH_COLOR_HPP_INCLUDED

#include "base.hpp"
#include "../color.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace scenic
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
	scenic::color const
	color() const = 0;

	virtual void
	color(
		scenic::color const &) = 0;

	~with_color();
protected:
	with_color();
};
}
}
}
}

#endif