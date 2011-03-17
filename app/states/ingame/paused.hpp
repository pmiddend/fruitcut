#ifndef FRUITCUT_APP_STATES_INGAME_PAUSED_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INGAME_PAUSED_HPP_INCLUDED

#include "superstate.hpp"
#include "../../events/render_overlay.hpp"
#include "../../events/tick.hpp"
#include "../../events/toggle_pause.hpp"
#include "../../../pp/system.hpp"
#include "../../../pp/filter/inject_texture.hpp"
#include "../../../pp/filter/blur.hpp"
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
	public boost::statechart::state<paused,superstate>
{
public:
	typedef
	boost::mpl::vector3
	<
		boost::statechart::custom_reaction<events::render_overlay>,
		boost::statechart::custom_reaction<events::tick>,
		boost::statechart::custom_reaction<events::toggle_pause>
	>
	reactions;
	explicit
	paused(
		my_context);

	boost::statechart::result
	react(
		events::render_overlay const &);

	boost::statechart::result
	react(
		events::tick const &);

	boost::statechart::result
	react(
		events::toggle_pause const &);

	~paused();
private:
	pp::system system_;
	pp::filter::inject_texture inject_texture_;
	pp::filter::blur blur_;
	sge::renderer::texture::planar_ptr current_texture_;
	pp::filter::blur::size_type blur_iterations_;
	pp::filter::blur::size_type const max_blur_iterations_;
	sge::time::timer blur_timer_;
};
}
}
}
}

#endif
