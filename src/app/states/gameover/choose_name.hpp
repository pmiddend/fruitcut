#ifndef FRUITCUT_APP_STATES_GAMEOVER_CHOOSE_NAME_HPP_INCLUDED
#define FRUITCUT_APP_STATES_GAMEOVER_CHOOSE_NAME_HPP_INCLUDED

#include "superstate.hpp"
#include "highscore_fwd.hpp"
#include "../../events/declare_transition_type.hpp"
#include "../../events/declare_transition_reaction.hpp"
#include <CEGUIEvent.h>
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <sge/cegui/toolbox/scoped_gui_sheet.hpp>
#include <boost/statechart/state.hpp>
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
	public boost::statechart::state<choose_name,gameover::superstate>
{
public:
	typedef
	boost::mpl::vector1
	<
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			gameover::highscore)
	>
	reactions;

	explicit
	choose_name(
		my_context);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		gameover::highscore);

	~choose_name();
private:
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
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
