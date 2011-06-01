#ifndef FRUITCUT_FRUITLIB_SCENIC_NODES_GUI_SYSTEM_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_NODES_GUI_SYSTEM_HPP_INCLUDED

#include "intrusive.hpp"
#include <sge/cegui/system_fwd.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/callback.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace scenic
{
namespace nodes
{
class gui_system
:
	public intrusive
{
public:
	explicit
	gui_system(
		sge::cegui::system &,
		sge::time::callback const &);

	~gui_system();
private:
	sge::time::timer frame_timer_;
	sge::cegui::system &system_;

	void
	update();

	void
	render();
};
}
}
}
}

#endif
