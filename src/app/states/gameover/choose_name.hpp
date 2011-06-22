#ifndef FRUITCUT_APP_STATES_GAMEOVER_CHOOSE_NAME_HPP_INCLUDED
#define FRUITCUT_APP_STATES_GAMEOVER_CHOOSE_NAME_HPP_INCLUDED

#include "superstate.hpp"
#include "highscore_fwd.hpp"
#include "../../events/make_transition.hpp"
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
	public boost::statechart::state<choose_name,superstate>
{
public:
	typedef
	boost::mpl::vector1
	<
		events::make_transition<gameover::highscore>::type
	>
	reactions;

	explicit
	choose_name(
		my_context);

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
