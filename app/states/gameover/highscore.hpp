#ifndef FRUITCUT_APP_STATES_GAMEOVER_HIGHSCORE_HPP_INCLUDED
#define FRUITCUT_APP_STATES_GAMEOVER_HIGHSCORE_HPP_INCLUDED

#include "superstate.hpp"
#include "../../events/gameover/quit_button_pushed.hpp"
#include "../../events/gameover/reset_button_pushed.hpp"
#include <CEGUI/CEGUIEvent.h>
#include <sge/cegui/toolbox/scoped_layout.hpp>
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
class highscore
:
	// The second argument has to be a complete type
	public boost::statechart::state<highscore,superstate>
{
public:
	typedef
	boost::mpl::vector2
	<
		boost::statechart::custom_reaction<events::gameover::quit_button_pushed>,
		boost::statechart::custom_reaction<events::gameover::reset_button_pushed>
	>
	reactions;

	explicit
	highscore(
		my_context);

	boost::statechart::result
	react(
		events::gameover::quit_button_pushed const &);

	boost::statechart::result
	react(
		events::gameover::reset_button_pushed const &);

	~highscore();
private:
	sge::cegui::toolbox::scoped_layout layout_;
	CEGUI::Event::Connection 
		quit_button_connection_,
		reset_button_connection_;

	bool
	quit_button_pushed(
		CEGUI::EventArgs const &);

	bool
	reset_button_pushed(
		CEGUI::EventArgs const &);
};
}
}
}
}

#endif
