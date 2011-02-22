#ifndef FRUITCUT_APP_STATES_INTRO_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INTRO_HPP_INCLUDED

#include "../machine.hpp"
#include "../events/render.hpp"
#include "../events/tick.hpp"
#include <sge/time/timer.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
{
namespace states
{
class intro
:
	// The second argument has to be a complete type
	public boost::statechart::state<intro,machine>
{
public:
	typedef
	boost::mpl::vector2
	<
		boost::statechart::custom_reaction<events::render>,
		boost::statechart::custom_reaction<events::tick>
	>
	reactions;

	explicit
	intro(
		my_context);

	boost::statechart::result
	react(
		events::render const &);

	boost::statechart::result
	react(
		events::tick const &);

	~intro();
private:
	sge::time::timer saturation_timer_;
	sge::time::timer intro_timer_;

};
}
}
}

#endif
