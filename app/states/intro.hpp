#ifndef FRUITCUT_APP_STATES_INTRO_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INTRO_HPP_INCLUDED

#include "../machine.hpp"
#include "../events/render.hpp"
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>

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
	boost::statechart::custom_reaction<events::render>
	reactions;

	explicit
	intro(
		my_context);

	boost::statechart::result
	react(
		events::render const &);

	~intro();
private:
	
};
}
}
}

#endif
