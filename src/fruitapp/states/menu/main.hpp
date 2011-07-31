#ifndef FRUITAPP_STATES_MENU_MAIN_HPP_INCLUDED
#define FRUITAPP_STATES_MENU_MAIN_HPP_INCLUDED

#include "superstate.hpp"
#include "settings_fwd.hpp"
#include "highscore_fwd.hpp"
#include "../../gui/button.hpp"
#include "../../logo.hpp"
#include "../../events/declare_transition_type.hpp"
#include "../../events/declare_transition_reaction.hpp"
#include "../ingame/superstate_fwd.hpp"
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <sge/cegui/toolbox/scoped_gui_sheet.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/statechart/state.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitapp
{
namespace states
{
namespace menu
{
class main
:
	// The second argument has to be a complete type
	public boost::statechart::state<main,menu::superstate>
{
FCPPT_NONCOPYABLE(
	main);
public:
	typedef
	boost::mpl::vector3
	<
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			ingame::superstate),
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::settings),
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::highscore)
	>
	reactions;

	explicit
	main(
		my_context);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		ingame::superstate);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::settings);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::highscore);

	~main();
private:
	fruitapp::logo logo_;
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	gui::button settings_button_;
	gui::button highscore_button_;
	gui::button quit_button_;
	gui::button start_button_;
	fcppt::signal::scoped_connection settings_button_connection_;
	fcppt::signal::scoped_connection highscore_button_connection_;
	fcppt::signal::scoped_connection quit_button_connection_;
	fcppt::signal::scoped_connection start_button_connection_;
};
}
}
}

#endif
