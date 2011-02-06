#ifndef FRUITCUT_APP_STATES_RUNNING_HPP_INCLUDED
#define FRUITCUT_APP_STATES_RUNNING_HPP_INCLUDED

#include "ingame.hpp"
#include "../events/render.hpp"
#include "../events/tick.hpp"
#include "../events/toggle_pause.hpp"
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
{
namespace states
{
class running
:
	// The second argument has to be a complete type
	public boost::statechart::state<running,ingame>
{
public:
	typedef
	boost::mpl::vector3
	<
		boost::statechart::custom_reaction<events::render>,
		boost::statechart::custom_reaction<events::tick>,
		boost::statechart::custom_reaction<events::toggle_pause>
	>
	reactions;
	explicit
	running(
		my_context);

	boost::statechart::result
	react(
		events::render const &);

	boost::statechart::result
	react(
		events::tick const &);

	boost::statechart::result
	react(
		events::toggle_pause const &);

	~running();
private:
	sge::renderer::state::scoped scoped_render_state_;
};
}
}
}

#endif
