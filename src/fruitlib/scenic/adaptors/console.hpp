#ifndef FRUITCUT_FRUITLIB_SCENIC_ADAPTORS_CONSOLE_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_ADAPTORS_CONSOLE_HPP_INCLUDED

#include "../node.hpp"
#include "../parent_fwd.hpp"
#include "../events/render_fwd.hpp"
#include <sge/console/gfx_fwd.hpp>
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
		scenic::parent const &parent,
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
}

#endif
