#ifndef FRUITCUT_FRUITLIB_SCENIC_ADAPTORS_LINE_DRAWER_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_ADAPTORS_LINE_DRAWER_HPP_INCLUDED

#include "../node.hpp"
#include "../optional_parent.hpp"
#include "../events/render_fwd.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/line_drawer/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace scenic
{
namespace adaptors
{
class line_drawer
:
	public scenic::node<line_drawer>
{
FCPPT_NONCOPYABLE(
	line_drawer);
public:
	typedef
	boost::mpl::vector1<events::render>
	scene_reactions;

	explicit
	line_drawer(
		scenic::optional_parent const &,
		sge::line_drawer::object &,
		sge::renderer::device *);

	~line_drawer();

	void
	react(
		events::render const &);
private:
	sge::line_drawer::object &object_;
	sge::renderer::device *renderer_;
};
}
}
}
}

#endif
