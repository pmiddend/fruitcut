#ifndef FRUITCUT_APP_STATES_INGAME_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INGAME_HPP_INCLUDED

#include "../machine.hpp"
#include "running_fwd.hpp"
#include <boost/statechart/state.hpp>

namespace fruitcut
{
namespace app
{
namespace states
{
class ingame
:
	// The second argument has to be a complete type, the third one
	// doesn't have to be
	public boost::statechart::state<ingame,machine,running>
{
public:
	explicit
	ingame(
		my_context);

	~ingame();
};
}
}
}

#endif
