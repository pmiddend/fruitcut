#ifndef FRUITCUT_APP_STATES_PAUSED_HPP_INCLUDED
#define FRUITCUT_APP_STATES_PAUSED_HPP_INCLUDED

#include "ingame.hpp"
#include "../events/render_overlay.hpp"
#include "../events/tick.hpp"
#include "../events/toggle_pause.hpp"
#include "../../pp/system.hpp"
#include "../../pp/filter/inject_texture.hpp"
#include "../../pp/filter/blur.hpp"
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <sge/renderer/texture_ptr.hpp>

namespace fruitcut
{
namespace app
{
namespace states
{
class paused
:
	// The second argument has to be a complete type
	public boost::statechart::state<paused,ingame>
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
	sge::renderer::texture_ptr current_texture_;
};
}
}
}

#endif
