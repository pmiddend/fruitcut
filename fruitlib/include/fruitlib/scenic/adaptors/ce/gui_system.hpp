#ifndef FRUITLIB_SCENIC_ADAPTORS_CE_GUI_SYSTEM_HPP_INCLUDED
#define FRUITLIB_SCENIC_ADAPTORS_CE_GUI_SYSTEM_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/delta/callback.hpp>
#include <fruitlib/scenic/delta/clock.hpp>
#include <fruitlib/scenic/delta/timer.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/cegui/system_fwd.hpp>
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
namespace ce
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

	FRUITLIB_DETAIL_SYMBOL
	explicit
	gui_system(
		scenic::optional_parent const &,
		sge::cegui::system &,
		scenic::delta::callback const &);

	FRUITLIB_DETAIL_SYMBOL
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
}

#endif
