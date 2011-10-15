#ifndef FRUITLIB_SCENIC_ADAPTORS_GUI_SYSTEM_HPP_INCLUDED
#define FRUITLIB_SCENIC_ADAPTORS_GUI_SYSTEM_HPP_INCLUDED

#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fruitlib/scenic/delta/callback.hpp>
#include <fruitlib/scenic/delta/timer.hpp>
#include <fruitlib/scenic/delta/clock.hpp>
#include <sge/cegui/system.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitlib
{
namespace scenic
{
namespace adaptors
{
class gui_system
:
	public scenic::node<gui_system>
{
FCPPT_NONCOPYABLE(
	gui_system);
public:
	typedef
	boost::mpl::vector2<events::render,events::update>
	scene_reactions;

	explicit
	gui_system(
		scenic::optional_parent const &,
		sge::cegui::system &,
		scenic::delta::callback const &);

	~gui_system();

	void
	react(
		events::update const &);

	void
	react(
		events::render const &);
private:
	scenic::delta::clock clock_;
	scenic::delta::timer timer_;
	sge::cegui::system &system_;
};
}
}
}

#endif
