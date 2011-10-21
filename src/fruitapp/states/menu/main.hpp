#ifndef FRUITAPP_STATES_MENU_MAIN_HPP_INCLUDED
#define FRUITAPP_STATES_MENU_MAIN_HPP_INCLUDED

#include <fruitapp/logo.hpp>
#include <fruitapp/events/declare_transition_reaction.hpp>
#include <fruitapp/events/declare_transition_type.hpp>
#include <fruitapp/gui/button.hpp>
#include <fruitapp/states/ingame/superstate_fwd.hpp>
#include <fruitapp/states/menu/highscore_fwd.hpp>
#include <fruitapp/states/menu/settings_fwd.hpp>
#include <fruitapp/states/menu/superstate.hpp>
#include <sge/cegui/toolbox/scoped_gui_sheet.hpp>
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/statechart/state.hpp>
#include <fcppt/config/external_end.hpp>


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
