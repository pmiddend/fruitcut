#ifndef FRUITLIB_SCENIC_EVENTS_UPDATE_HPP_INCLUDED
#define FRUITLIB_SCENIC_EVENTS_UPDATE_HPP_INCLUDED

#include <fruitlib/scenic/events/base.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitlib
{
namespace scenic
{
namespace events
{
class update
:
	public events::base
{
FCPPT_NONCOPYABLE(
	update);
public:
	explicit
	update();

	~update();
};
}
}
}

#endif
