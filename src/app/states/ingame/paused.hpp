#ifndef FRUITCUT_APP_STATES_INGAME_PAUSED_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INGAME_PAUSED_HPP_INCLUDED

#include "superstate.hpp"
#include "../../scoped_scene_activation.hpp"
#include "../../scoped_time_factor.hpp"
#include "../../events/render_overlay.hpp"
#include "../../events/tick.hpp"
#include "../../events/toggle_pause.hpp"
#include "../../../pp/system.hpp"
#include "../../../pp/filter/inject_texture.hpp"
#include "../../../pp/filter/blur.hpp"
#include "../../../scenic/nodes/intrusive.hpp"
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/time/timer.hpp>

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
	public scenic::nodes::intrusive
{
public:
	typedef
	boost::mpl::vector1
	<
		boost::statechart::custom_reaction<events::toggle_pause>
	>
	reactions;

	explicit
	paused(
		my_context);

	boost::statechart::result
	react(
		events::toggle_pause const &);

	~paused();
private:
	app::scoped_time_factor time_factor_;
	app::scoped_scene_activation scene_deactivation_;
	pp::system system_;
	pp::filter::inject_texture inject_texture_;
	pp::filter::blur blur_;
	sge::renderer::texture::planar_ptr current_texture_;
	pp::filter::blur::size_type blur_iterations_;
	pp::filter::blur::size_type const max_blur_iterations_;
	sge::time::timer blur_timer_;

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
