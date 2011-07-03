#ifndef FRUITCUT_APP_STATES_MENU_SETTINGS_HPP_INCLUDED
#define FRUITCUT_APP_STATES_MENU_SETTINGS_HPP_INCLUDED

#include "superstate.hpp"
#include "main_fwd.hpp"
#include "../../events/declare_transition_type.hpp"
#include "../../events/declare_transition_reaction.hpp"
#include "../../gui/button.hpp"
#include "../../gui/progress_slider.hpp"
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
	gui::button main_menu_button_;
	fcppt::signal::scoped_connection main_menu_button_connection_;
	gui::progress_slider music_volume_slider_;
	fcppt::signal::scoped_connection music_volume_connection_;
	gui::progress_slider effects_volume_slider_;
	fcppt::signal::scoped_connection effects_volume_connection_;
	gui::progress_slider splatter_slider_;
	fcppt::signal::scoped_connection splatter_connection_;

	void
	music_volume_callback(
		gui::progress_slider::value_type);

	void
	effects_volume_callback(
		gui::progress_slider::value_type);

	void
	particle_callback(
		gui::progress_slider::value_type);
};
}
}
}
}

#endif
