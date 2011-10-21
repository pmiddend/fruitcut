#ifndef FRUITLIB_SCENIC_EVENTS_RENDER_HPP_INCLUDED
#define FRUITLIB_SCENIC_EVENTS_RENDER_HPP_INCLUDED

#include <fruitlib/scenic/events/base.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitlib
{
namespace scenic
{
namespace events
{
class render
:
	public events::base
{
FCPPT_NONCOPYABLE(
	render);
public:
	explicit
	render();

	~render();
};
}
}
}

#endif
