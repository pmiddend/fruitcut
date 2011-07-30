#ifndef FRUITCUT_FRUITLIB_SCENIC_EVENTS_BASE_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_EVENTS_BASE_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace scenic
{
namespace events
{
class base
{
FCPPT_NONCOPYABLE(
	base);
public:
	virtual
	~base();
protected:
	base();
};
}
}
}
}

#endif
