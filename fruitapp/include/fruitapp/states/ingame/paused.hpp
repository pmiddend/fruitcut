#ifndef FRUITAPP_STATES_INGAME_PAUSED_HPP_INCLUDED
#define FRUITAPP_STATES_INGAME_PAUSED_HPP_INCLUDED

#include <fruitapp/scoped_scene_activation.hpp>
#include <fruitapp/scoped_time_factor.hpp>
#include <fruitapp/events/declare_transition_reaction.hpp>
#include <fruitapp/events/declare_transition_type.hpp>
#include <fruitapp/gui/dialogs/ingame_menu_unique_ptr.hpp>
#include <fruitapp/postprocessing/subsystems/paused_unique_ptr.hpp>
#include <fruitapp/states/ingame/running_fwd.hpp>
#include <fruitapp/states/ingame/superstate.hpp>
#include <fruitapp/states/menu/main_fwd.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/timer/basic.hpp>
#include <sge/timer/clocks/standard.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/statechart/state.hpp>
#include <fcppt/config/external_end.hpp>


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
	public boost::statechart::state<paused,superstate>
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

	explicit
	paused(
		my_context);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		ingame::running);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::main);

	~paused();
private:
	fruitapp::scoped_time_factor time_factor_;
	//fruitapp::scoped_scene_activation scene_deactivation_;
	fruitapp::postprocessing::subsystems::paused_unique_ptr const paused_postprocessing_;
	fcppt::signal::auto_connection transit_to_running_connection_;
	fruitapp::gui::dialogs::ingame_menu_unique_ptr ingame_menu_;
	fcppt::signal::auto_connection continue_connection_;
	fcppt::signal::auto_connection main_menu_connection_;
	fcppt::signal::auto_connection quit_connection_;
};
}
}
}

#endif
