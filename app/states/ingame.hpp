#ifndef FRUITCUT_APP_STATES_INGAME_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INGAME_HPP_INCLUDED

#include "../machine.hpp"
#include "running_fwd.hpp"
#include "../fruit_prototype.hpp"
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state.hpp>
#include <vector>

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
private:
	typedef
	std::vector<fruit_prototype>
	prototype_sequence;

	fcppt::signal::scoped_connection toggle_pause_connection_;
	prototype_sequence prototypes_;
};
}
}
}

#endif
