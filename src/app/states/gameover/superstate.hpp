#ifndef FRUITCUT_APP_STATES_GAMEOVER_SUPERSTATE_HPP_INCLUDED
#define FRUITCUT_APP_STATES_GAMEOVER_SUPERSTATE_HPP_INCLUDED

#include "choose_name_fwd.hpp"
#include "../../machine.hpp"
#include "../../../fruitlib/scenic/nodes/gui_system.hpp"
#include <sge/cegui/default_cursor.hpp>
#include <sge/cegui/default_keyboard.hpp>
#include <CEGUIString.h>
#include <boost/statechart/state.hpp>

namespace fruitcut
{
namespace app
{
namespace states
{
namespace gameover
{
class superstate
:
	// The second argument has to be a complete type
	public boost::statechart::state<superstate,machine,choose_name>
{
public:
	explicit
	superstate(
		my_context);

	void
	name(
		CEGUI::String const &);

	CEGUI::String const &
	name() const;

	~superstate();
private:
	fruitlib::scenic::nodes::gui_system gui_node_;
	sge::cegui::default_keyboard gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
	CEGUI::String name_;
};
}
}
}
}

#endif
