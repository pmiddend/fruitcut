#ifndef FRUITCUT_SCENIC_NODES_BASE_HPP_INCLUDED
#define FRUITCUT_SCENIC_NODES_BASE_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace scenic
{
namespace nodes
{
class base
{
FCPPT_NONCOPYABLE(
	base);
public:
	virtual void
	update() = 0;

	virtual void
	render() = 0;

	virtual ~base();
protected:
	base();
};
}
}
}

#endif
