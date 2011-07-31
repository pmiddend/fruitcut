#ifndef FRUITAPP_STATES_INGAME_PAUSED_HPP_INCLUDED
#define FRUITAPP_STATES_INGAME_PAUSED_HPP_INCLUDED

#include "superstate.hpp"
#include "running_fwd.hpp"
#include <fruitapp/states/menu/main_fwd.hpp>
#include <fruitapp/gui/button.hpp>
#include <fruitapp/scoped_scene_activation.hpp>
#include <fruitapp/scoped_pp_activation.hpp>
#include <fruitapp/scoped_time_factor.hpp>
#include <fruitapp/events/declare_transition_type.hpp>
#include <fruitapp/events/declare_transition_reaction.hpp>
#include <fruitlib/pp/system.hpp>
#include <fruitlib/pp/filter/inject_texture.hpp>
#include <fruitlib/pp/filter/blur.hpp>
#include <fruitlib/scenic/adaptors/gui_system.hpp>
#include <fruitlib/scenic/node.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/time/timer.hpp>
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <sge/cegui/default_keyboard.hpp>
#include <sge/cegui/default_cursor.hpp>
#include <sge/cegui/toolbox/scoped_gui_sheet.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/statechart/state.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitapp
{
namespace states
{
namespace ingame
{
/**
	So, how does the pause mode work?

	Ok, so as seen in the fruitapp::renderable class, the "normal" rendering
	procedure involves rendering all the 3D stuff into a texture, then
	applying post processing on this texture and then rendering this
	texture, along with the overlay stuff (the HUD, for example).

	With pause mode, we don't want to render the 3D stuff. What we want
	is to take the "last" rendering of the scene before the pause mode
	was entered and incrementally blur this texture 'n' times.

	To blur something, we set up a post processing chain (since we
	already have a blur shader which is used for the bloom
	effect). However, the preprocessing needs input. Usually, the input
	is a render callback which renders the scene into a texture, which
	is then used as the main input of the pipeline. 

	Using a special filter, the "inject_texture" filter, we can use an
	existing texture as the pipeline input. The first time we blur, we
	use the "old scene rendering" as the input texture. The pipeline
	will blur the texture and output a new, blurred, t texture 't'. In
	the next blur pass, we use this 't' texture as input. And so on (we
	have a feedback look in the pipeline).

	There's one more bit to mention: If we want to override the normal
	rendering behaviour when we are in pause mode, don't we have to
	disable the normal behaviour in some way? Indeed we do. We can disable two things:

	1. The scene (so no 3D stuff is rendered to the texture anymore,
	   saving performance)
	2. The postprocessing pipeline. The texture will not be updated and
	   not rendered anymore
 */
class paused
:
	// The second argument has to be a complete type
	public boost::statechart::state<paused,superstate>,
	public fruitlib::scenic::node<paused>
{
FCPPT_NONCOPYABLE(
	paused);
public:
	typedef
	boost::mpl::vector2
	<
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			ingame::running),
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::main)
	>
	reactions;

	typedef
	boost::mpl::vector2<fruitlib::scenic::events::render,fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	paused(
		my_context);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		ingame::running);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::main);

	~paused();

	void
	react(
		fruitlib::scenic::events::render const &);

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	fruitapp::scoped_time_factor time_factor_;
	fruitapp::scoped_scene_activation scene_deactivation_;
	fruitapp::scoped_pp_activation pp_deactivation_;
	fruitlib::pp::system system_;
	fruitlib::pp::filter::inject_texture inject_texture_;
	fruitlib::pp::filter::blur blur_;
	sge::renderer::texture::planar_ptr current_texture_;
	fruitlib::pp::filter::blur::size_type blur_iterations_;
	fruitlib::pp::filter::blur::size_type const max_blur_iterations_;
	sge::time::timer blur_timer_;
	fcppt::signal::scoped_connection transit_to_running_connection_;
	fruitlib::scenic::adaptors::gui_system gui_node_;
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
};
}
}
}

#endif
