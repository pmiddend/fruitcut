#ifndef FRUITCUT_FRUITLIB_SCENIC_NODES_WITH_LIFETIME_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_NODES_WITH_LIFETIME_HPP_INCLUDED

#include "base.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace scenic
{
namespace nodes
{
class with_lifetime
:
	public virtual base
{
FCPPT_NONCOPYABLE(
	with_lifetime);
public:
	virtual bool
	dead() const = 0;
protected:
	with_lifetime();
};
}
}
}
}

#endif
