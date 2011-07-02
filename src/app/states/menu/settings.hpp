#ifndef FRUITCUT_APP_STATES_MENU_SETTINGS_HPP_INCLUDED
#define FRUITCUT_APP_STATES_MENU_SETTINGS_HPP_INCLUDED

#include "superstate.hpp"
#include "main_fwd.hpp"
#include "../../events/declare_transition_type.hpp"
#include "../../events/declare_transition_reaction.hpp"
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <sge/cegui/toolbox/scoped_gui_sheet.hpp>
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
class settings
:
	// The second argument has to be a complete type
	public boost::statechart::state<settings,superstate>
{
public:
	typedef
	boost::mpl::vector1
	<
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::main)
	>
	reactions;

	explicit
	settings(
		my_context);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::main);

	~settings();
private:
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	CEGUI::Event::ScopedConnection main_menu_button_connection_;
	CEGUI::Event::ScopedConnection music_slider_pulled_connection_;
	CEGUI::Event::ScopedConnection effects_slider_pulled_connection_;
	CEGUI::Event::ScopedConnection splatter_slider_pulled_connection_;

	bool
	main_menu_button_pushed(
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
