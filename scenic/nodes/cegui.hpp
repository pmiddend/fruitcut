#ifndef FRUITCUT_SCENIC_NODES_CEGUI_HPP_INCLUDED
#define FRUITCUT_SCENIC_NODES_CEGUI_HPP_INCLUDED

#include "intrusive.hpp"
#include <sge/cegui/system_fwd.hpp>
#include <sge/time/callback.hpp>
#include <sge/time/timer.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace scenic
{
namespace nodes
{
class cegui
:
	public intrusive
{
FCPPT_NONCOPYABLE(
	cegui);
public:
	explicit
	cegui(
		sge::cegui::system &,
		sge::time::callback const &);

	~cegui();
protected:
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

#endif
