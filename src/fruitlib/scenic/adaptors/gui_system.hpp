#ifndef FRUITCUT_FRUITLIB_SCENIC_ADAPTORS_GUI_SYSTEM_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_ADAPTORS_GUI_SYSTEM_HPP_INCLUDED

#include "../node.hpp"
#include "../optional_parent.hpp"
#include "../events/render_fwd.hpp"
#include "../events/update_fwd.hpp"
#include <sge/cegui/system.hpp>
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
		sge::cegui::system &);

	~gui_system();

	void
	react(
		events::update const &);

	void
	react(
		events::render const &);
private:
	sge::cegui::system &system_;
};
}
}
}
}

#endif
