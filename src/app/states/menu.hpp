#ifndef FRUITCUT_APP_STATES_MENU_HPP_INCLUDED
#define FRUITCUT_APP_STATES_MENU_HPP_INCLUDED

#include "../machine.hpp"
#include "../logo.hpp"
#include "../events/make_transition.hpp"
#include "../../fruitlib/scenic/nodes/intrusive.hpp"
#include "../../fruitlib/scenic/nodes/gui_system.hpp"
#include "ingame/running_fwd.hpp"
#include <sge/time/timer.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/cegui/default_cursor.hpp>
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <sge/cegui/toolbox/scoped_gui_sheet.hpp>
#include <sge/cegui/default_keyboard.hpp>
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
class menu
:
	// The second argument has to be a complete type
	public boost::statechart::state<menu,machine>,
	public fruitlib::scenic::nodes::intrusive
{
public:
	typedef
	boost::mpl::vector1
	<
		events::make_transition<ingame::running>::type
	>
	reactions;

	explicit
	menu(
		my_context);

	~menu();
private:
	sge::renderer::state::scoped scoped_render_state_;
	sge::time::timer saturation_timer_;
	fcppt::signal::scoped_connection viewport_change_connection_;
	logo logo_;
	fruitlib::scenic::nodes::gui_system gui_node_;
	sge::cegui::default_keyboard gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	CEGUI::Event::ScopedConnection quit_button_connection_;
	CEGUI::Event::ScopedConnection start_button_connection_;
	CEGUI::Event::ScopedConnection music_slider_pulled_connection_;
	CEGUI::Event::ScopedConnection effects_slider_pulled_connection_;

	void
	viewport_change();

	void
	update();

	void
	render();

	bool
	quit_button_pushed(
		CEGUI::EventArgs const &);

	bool
	start_button_pushed(
		CEGUI::EventArgs const &);

	bool
	music_slider_pulled(
		CEGUI::EventArgs const &);

	bool
	effects_slider_pulled(
		CEGUI::EventArgs const &);
};
}
}
}

#endif
