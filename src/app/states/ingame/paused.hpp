#ifndef FRUITCUT_APP_STATES_INGAME_PAUSED_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INGAME_PAUSED_HPP_INCLUDED

#include "superstate.hpp"
#include "running_fwd.hpp"
#include "../menu/main_fwd.hpp"
#include "../../gui/button.hpp"
#include "../../scoped_scene_activation.hpp"
#include "../../scoped_time_factor.hpp"
#include "../../events/declare_transition_type.hpp"
#include "../../events/declare_transition_reaction.hpp"
#include "../../../fruitlib/pp/system.hpp"
#include "../../../fruitlib/pp/filter/inject_texture.hpp"
#include "../../../fruitlib/pp/filter/blur.hpp"
#include "../../../fruitlib/scenic/nodes/intrusive.hpp"
#include "../../../fruitlib/scenic/nodes/gui_system.hpp"
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/time/timer.hpp>
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <sge/cegui/default_keyboard.hpp>
#include <sge/cegui/default_cursor.hpp>
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
namespace ingame
{
class paused
:
	// The second argument has to be a complete type
	public boost::statechart::state<paused,superstate>,
	public fruitlib::scenic::nodes::intrusive
{
public:
	typedef
	boost::mpl::vector2
	<
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			ingame::running),
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::main)
	>
	reactions;

	explicit
	paused(
		my_context);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		ingame::running);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::main);

	~paused();
private:
	app::scoped_time_factor time_factor_;
	app::scoped_scene_activation scene_deactivation_;
	fruitlib::pp::system system_;
	fruitlib::pp::filter::inject_texture inject_texture_;
	fruitlib::pp::filter::blur blur_;
	sge::renderer::texture::planar_ptr current_texture_;
	fruitlib::pp::filter::blur::size_type blur_iterations_;
	fruitlib::pp::filter::blur::size_type const max_blur_iterations_;
	sge::time::timer blur_timer_;
	fcppt::signal::scoped_connection transit_to_running_connection_;

	fruitlib::scenic::nodes::gui_system gui_node_;
	sge::cegui::default_keyboard gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	gui::button continue_button_;
	gui::button main_menu_button_;
	gui::button quit_button_;
	fcppt::signal::scoped_connection continue_connection_;
	fcppt::signal::scoped_connection main_menu_connection_;
	fcppt::signal::scoped_connection quit_connection_;

	void
	render();

	void
	update();
};
}
}
}
}

#endif
