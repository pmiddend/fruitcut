#ifndef FRUITLIB_SCENIC_ADAPTORS_CONSOLE_HPP_INCLUDED
#define FRUITLIB_SCENIC_ADAPTORS_CONSOLE_HPP_INCLUDED

#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/console/gfx_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitlib
{
namespace scenic
{
namespace adaptors
{
class console
:
	public scenic::node<console>
{
FCPPT_NONCOPYABLE(
	console);
public:
	typedef
	boost::mpl::vector1<events::render>
	scene_reactions;

	explicit
	console(
		scenic::optional_parent const &parent,
		sge::console::gfx &);

	~console();

	void
	react(
		events::render const &);
private:
	sge::console::gfx &gfx_;
};
}
}
}

#endif
