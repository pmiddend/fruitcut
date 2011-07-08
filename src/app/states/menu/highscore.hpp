#ifndef FRUITCUT_APP_STATES_MENU_HIGHSCORE_HPP_INCLUDED
#define FRUITCUT_APP_STATES_MENU_HIGHSCORE_HPP_INCLUDED

#include "superstate.hpp"
#include "settings_fwd.hpp"
#include "../../gui/button.hpp"
#include "../../events/declare_transition_type.hpp"
#include "../../events/declare_transition_reaction.hpp"
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <sge/cegui/toolbox/scoped_gui_sheet.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
{
namespace states
{
namespace menu
{
class highscore
:
	// The second argument has to be a complete type
	public boost::statechart::state<highscore,menu::superstate>
{
FCPPT_NONCOPYABLE(
	highscore);
public:
	typedef
	boost::mpl::vector1
	<
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::main)
	>
	reactions;

	explicit
	highscore(
		my_context);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::main);

	~highscore();
private:
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	gui::button main_menu_button_;
	gui::button quit_button_;
	fcppt::signal::scoped_connection main_menu_button_connection_;
	fcppt::signal::scoped_connection quit_button_connection_;
};
}
}
}
}

#endif
