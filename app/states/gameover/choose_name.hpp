#ifndef FRUITCUT_APP_STATES_GAMEOVER_CHOOSE_NAME_HPP_INCLUDED
#define FRUITCUT_APP_STATES_GAMEOVER_CHOOSE_NAME_HPP_INCLUDED

#include "superstate.hpp"
#include "../../events/gameover/continue_button_pushed.hpp"
#include <CEGUI/CEGUIEvent.h>
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace CEGUI
{
class EventArgs;
}

namespace fruitcut
{
namespace app
{
namespace states
{
namespace gameover
{
class choose_name
:
	// The second argument has to be a complete type
	public boost::statechart::state<choose_name,superstate>
{
public:
	typedef
	boost::mpl::vector1
	<
		boost::statechart::custom_reaction<events::gameover::continue_button_pushed>
	>
	reactions;

	explicit
	choose_name(
		my_context);

	boost::statechart::result
	react(
		events::gameover::continue_button_pushed const &);

	~choose_name();
private:
	sge::cegui::toolbox::scoped_layout layout_;
	CEGUI::Event::ScopedConnection continue_button_connection_;

	bool
	continue_button_pushed(
		CEGUI::EventArgs const &);
};
}
}
}
}

#endif
