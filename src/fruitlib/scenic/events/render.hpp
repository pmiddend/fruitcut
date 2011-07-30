#ifndef FRUITCUT_FRUITLIB_SCENIC_EVENTS_RENDER_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_EVENTS_RENDER_HPP_INCLUDED

#include "base.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
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
}

#endif
