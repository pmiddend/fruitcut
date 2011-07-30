#ifndef FRUITCUT_APP_STATES_GAMEOVER_RANKING_HPP_INCLUDED
#define FRUITCUT_APP_STATES_GAMEOVER_RANKING_HPP_INCLUDED

#include "superstate.hpp"
#include "../menu/main_fwd.hpp"
#include "../menu/highscore_fwd.hpp"
#include "../../events/declare_transition_type.hpp"
#include "../../events/declare_transition_reaction.hpp"
#include "../../gui/button.hpp"
#include "../../gui/table/view.hpp"
#include "../../../fruitlib/scenic/node.hpp"
#include "../../../fruitlib/scenic/events/update_fwd.hpp"
#include "../../highscore/provider_sequence.hpp"
#include "../../highscore/post_model.hpp"
#include "../../gui/table/view.hpp"
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <sge/cegui/toolbox/scoped_gui_sheet.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/string.hpp>
#include <boost/statechart/state.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
{
namespace states
{
namespace gameover
{
class ranking
:
	// The second argument has to be a complete type
	public boost::statechart::state<ranking,superstate>,
	public fruitlib::scenic::node<ranking>
{
FCPPT_NONCOPYABLE(
	ranking);
public:
	typedef
	boost::mpl::vector2
	<
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::main),
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::highscore)
	>
	reactions;

	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	ranking(
		my_context);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::main);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::highscore);

	~ranking();
		
	void
	react(
		fruitlib::scenic::events::update const &);
private:
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	gui::button main_menu_button_;
	gui::button highscore_button_;
	gui::button quit_button_;
	fcppt::signal::scoped_connection main_menu_button_connection_;
	fcppt::signal::scoped_connection highscore_button_connection_;
	fcppt::signal::scoped_connection quit_button_connection_;
	app::highscore::provider_sequence providers_;
	highscore::post_model post_model_;
	gui::table::view table_view_;
	fcppt::signal::scoped_connection message_received_connection_;
	fcppt::signal::scoped_connection error_received_connection_;

	void
	message_received(
		fcppt::string const &);

	void
	error_received(
		fcppt::string const &);
};
}
}
}
}

#endif
