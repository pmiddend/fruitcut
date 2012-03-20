#ifndef FRUITAPP_STATES_MENU_SUPERSTATE_HPP_INCLUDED
#define FRUITAPP_STATES_MENU_SUPERSTATE_HPP_INCLUDED

#include <fruitapp/machine.hpp>
#include <fruitapp/states/menu/main_fwd.hpp>
#include <fruitlib/scenic/adaptors/gui_system.hpp>
#include <sge/cegui/default_cursor.hpp>
#include <sge/cegui/default_keyboard.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/statechart/state.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
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
FCPPT_NONCOPYABLE(
	superstate);
public:
	explicit
	superstate(
		my_context);

	virtual ~superstate();
private:
	fruitlib::scenic::adaptors::gui_system gui_node_;
	sge::cegui::default_keyboard gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
	fcppt::signal::scoped_connection escape_connection_;
};
}
}
}

#endif
