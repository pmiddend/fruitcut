#ifndef FRUITCUT_FRUITLIB_SCENIC_NODES_LINE_DRAWER_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_NODES_LINE_DRAWER_HPP_INCLUDED

#include "intrusive.hpp"
#include <sge/line_drawer/object_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace scenic
{
namespace nodes
{
class line_drawer
:
	public intrusive
{
FCPPT_NONCOPYABLE(
	line_drawer);
public:
	explicit
	line_drawer(
		sge::line_drawer::object &,
		sge::renderer::device*);

	~line_drawer();
private:
	sge::line_drawer::object &object_;
	sge::renderer::device * const renderer_;

	void
	render();

	void
	update();
};
}
}
}
}

#endif
