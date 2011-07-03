#ifndef FRUITCUT_APP_STATES_GAMEOVER_HIGHSCORE_HPP_INCLUDED
#define FRUITCUT_APP_STATES_GAMEOVER_HIGHSCORE_HPP_INCLUDED

#include "superstate.hpp"
#include "../menu/main_fwd.hpp"
#include "../ingame/superstate_fwd.hpp"
#include "../../events/declare_transition_type.hpp"
#include "../../events/declare_transition_reaction.hpp"
#include "../../gui/button.hpp"
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
namespace gameover
{
class highscore
:
	// The second argument has to be a complete type
	public boost::statechart::state<highscore,superstate>
{
public:
	typedef
	boost::mpl::vector2
	<
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::main),
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			ingame::superstate)
	>
	reactions;

	explicit
	highscore(
		my_context);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::main);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		ingame::superstate);

	~highscore();
private:
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	gui::button quit_button_;
	gui::button reset_button_;
	gui::button main_menu_button_;
	fcppt::signal::scoped_connection quit_button_connection_;
	fcppt::signal::scoped_connection reset_button_connection_;
	fcppt::signal::scoped_connection main_menu_button_connection_;
		
};
}
}
}
}

#endif
