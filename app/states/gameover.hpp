#ifndef FRUITCUT_APP_STATES_GAMEOVER_HPP_INCLUDED
#define FRUITCUT_APP_STATES_GAMEOVER_HPP_INCLUDED

#include "../machine.hpp"
#include "../events/render.hpp"
#include "../events/render_overlay.hpp"
#include "../events/tick.hpp"
#include <sge/time/timer.hpp>
#include <sge/cegui/default_cursor.hpp>
#include <sge/cegui/default_keyboard.hpp>
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/scoped_ptr.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
{
namespace states
{
class gameover
:
	// The second argument has to be a complete type
	public boost::statechart::state<gameover,machine>
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
	gameover(
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

	~gameover();
private:
	sge::time::timer frame_timer_;
	sge::cegui::default_keyboard gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
	fcppt::scoped_ptr<sge::cegui::toolbox::scoped_layout> current_layout_;
};
}
}
}

#endif
