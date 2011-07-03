#ifndef FRUITCUT_APP_STATES_MENU_MAIN_HPP_INCLUDED
#define FRUITCUT_APP_STATES_MENU_MAIN_HPP_INCLUDED

#include "superstate.hpp"
#include "settings_fwd.hpp"
#include "../../logo.hpp"
#include "../../events/declare_transition_type.hpp"
#include "../../events/declare_transition_reaction.hpp"
#include "../ingame/superstate_fwd.hpp"
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <sge/cegui/toolbox/scoped_gui_sheet.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <CEGUIEvent.h>

namespace fruitcut
{
namespace app
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
public:
	typedef
	boost::mpl::vector2
	<
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			ingame::superstate),
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::settings)
	>
	reactions;

	explicit
	main(
		my_context);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		ingame::superstate);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::settings);

	~main();
private:
	fcppt::signal::scoped_connection viewport_change_connection_;
	app::logo logo_;
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	CEGUI::Event::ScopedConnection quit_button_connection_;
	CEGUI::Event::ScopedConnection start_button_connection_;
	CEGUI::Event::ScopedConnection settings_button_connection_;
	CEGUI::Event::ScopedConnection music_slider_pulled_connection_;
	CEGUI::Event::ScopedConnection effects_slider_pulled_connection_;
	CEGUI::Event::ScopedConnection splatter_slider_pulled_connection_;

	bool
	quit_button_pushed(
		CEGUI::EventArgs const &);

	bool
	start_button_pushed(
		CEGUI::EventArgs const &);

	bool
	settings_button_pushed(
		CEGUI::EventArgs const &);

	bool
	music_slider_pulled(
		CEGUI::EventArgs const &);

	bool
	effects_slider_pulled(
		CEGUI::EventArgs const &);

	bool
	splatter_slider_pulled(
		CEGUI::EventArgs const &);
};
}
}
}
}

#endif
