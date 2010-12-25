#ifndef FRUITCUT_SANDBOX_CUT_TEST_STATES_FREELOOK_HPP_INCLUDED
#define FRUITCUT_SANDBOX_CUT_TEST_STATES_FREELOOK_HPP_INCLUDED

#include "ingame.hpp"
#include "../events/tick.hpp"
#include "../events/toggle_mode.hpp"
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
//#include <boost/statechart/transition.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace sandbox
{
namespace cut_test
{
namespace states
{
class freelook
:
	public boost::statechart::state<freelook,ingame>
{
public:
	typedef 
	boost::mpl::vector2
	<
		boost::statechart::custom_reaction<events::tick>,
		boost::statechart::custom_reaction<events::toggle_mode>
		// I could do this, but it's a dependency nightmare
		//boost::statechart::transition<events::toggle_mode,cut>
	> 
	reactions;

	explicit
	freelook(
		my_context);

	boost::statechart::result
	react(
		events::tick const &);

	boost::statechart::result
	react(
		events::toggle_mode const &);
};
}
}
}
}

#endif
