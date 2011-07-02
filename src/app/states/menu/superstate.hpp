#ifndef FRUITCUT_APP_STATES_MENU_SUPERSTATE_HPP_INCLUDED
#define FRUITCUT_APP_STATES_MENU_SUPERSTATE_HPP_INCLUDED

#include "../../machine.hpp"
#include "main_fwd.hpp"
#include "../../../fruitlib/scenic/nodes/gui_system.hpp"
#include <sge/cegui/default_cursor.hpp>
#include <sge/cegui/default_keyboard.hpp>
#include <boost/statechart/state.hpp>

namespace fruitcut
{
namespace app
{
namespace states
{
namespace menu
{
class superstate
:
	// The second argument has to be a complete type
	public boost::statechart::state<superstate,machine,main>
{
public:
	explicit
	superstate(
		my_context);

	~superstate();
private:
	fruitlib::scenic::nodes::gui_system gui_node_;
	sge::cegui::default_keyboard gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
};
}
}
}
}

#endif
