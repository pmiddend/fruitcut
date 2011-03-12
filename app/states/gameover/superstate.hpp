#ifndef FRUITCUT_APP_STATES_GAMEOVER_SUPERSTATE_HPP_INCLUDED
#define FRUITCUT_APP_STATES_GAMEOVER_SUPERSTATE_HPP_INCLUDED

#include "choose_name_fwd.hpp"
#include "../../machine.hpp"
#include "../../events/render.hpp"
#include "../../events/render_overlay.hpp"
#include "../../events/tick.hpp"
#include <sge/time/timer.hpp>
#include <sge/cegui/default_cursor.hpp>
#include <sge/cegui/default_keyboard.hpp>
#include <CEGUI/CEGUIString.h>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
{
namespace states
{
namespace gameover
{
class superstate
:
	// The second argument has to be a complete type
	public boost::statechart::state<superstate,machine,choose_name>
{
public:
	typedef
	boost::mpl::vector3
	<
		boost::statechart::custom_reaction<events::render>,
		boost::statechart::custom_reaction<events::render_overlay>,
		boost::statechart::custom_reaction<events::tick>
	>
	reactions;

	explicit
	superstate(
		my_context);

	boost::statechart::result
	react(
		events::render const &);

	boost::statechart::result
	react(
		events::render_overlay const &);

	boost::statechart::result
	react(
		events::tick const &);

	void
	name(
		CEGUI::String const &);

	CEGUI::String const &
	name() const;

	~superstate();
private:
	sge::time::timer frame_timer_;
	sge::cegui::default_keyboard gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
	CEGUI::String name_;
};
}
}
}
}

#endif