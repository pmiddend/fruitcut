#ifndef FRUITCUT_SCENIC_NODES_CONSOLE_HPP_INCLUDED
#define FRUITCUT_SCENIC_NODES_CONSOLE_HPP_INCLUDED

#include "intrusive.hpp"
#include <sge/console/gfx_fwd.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace scenic
{
namespace nodes
{
class console
:
	public virtual intrusive
{
FCPPT_NONCOPYABLE(
	console);
public:
	explicit
	console(
		sge::console::gfx &);

	~console();
protected:
	sge::console::gfx &gfx_;

	void
	update();

	void
	render();
};
}
}
}

#endif
