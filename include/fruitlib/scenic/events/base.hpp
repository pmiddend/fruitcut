#ifndef FRUITLIB_SCENIC_EVENTS_BASE_HPP_INCLUDED
#define FRUITLIB_SCENIC_EVENTS_BASE_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>

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

#endif
