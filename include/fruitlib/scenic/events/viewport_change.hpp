#ifndef FRUITLIB_SCENIC_EVENTS_VIEWPORT_CHANGE_HPP_INCLUDED
#define FRUITLIB_SCENIC_EVENTS_VIEWPORT_CHANGE_HPP_INCLUDED

#include <fruitlib/scenic/events/base.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitlib
{
namespace scenic
{
namespace events
{
class viewport_change
:
	public events::base
{
FCPPT_NONCOPYABLE(
	viewport_change);
public:
	explicit
	viewport_change();

	~viewport_change();
};
}
}
}

#endif
