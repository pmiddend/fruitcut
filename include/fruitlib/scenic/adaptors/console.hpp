#ifndef FRUITLIB_SCENIC_ADAPTORS_CONSOLE_HPP_INCLUDED
#define FRUITLIB_SCENIC_ADAPTORS_CONSOLE_HPP_INCLUDED

#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/console/gfx_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace scenic
{
namespace adaptors
{
class console
:
	public fruitlib::scenic::node<console>
{
FCPPT_NONCOPYABLE(
	console);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::render>
	scene_reactions;

	console(
		fruitlib::scenic::optional_parent const &parent,
		sge::console::gfx &);

	~console();

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	sge::console::gfx &gfx_;
};
}
}
}

#endif
