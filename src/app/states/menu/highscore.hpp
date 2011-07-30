#ifndef FRUITCUT_APP_STATES_MENU_HIGHSCORE_HPP_INCLUDED
#define FRUITCUT_APP_STATES_MENU_HIGHSCORE_HPP_INCLUDED

#include "superstate.hpp"
#include "../../../fruitlib/scenic/node.hpp"
#include "../../../fruitlib/scenic/parent_fwd.hpp"
#include "../../../fruitlib/scenic/events/update_fwd.hpp"
#include "../../highscore/provider_sequence.hpp"
#include "../../highscore/provider/object_base.hpp"
#include "../../highscore/provider/connection_base_ptr.hpp"
#include "../../highscore/get_model.hpp"
#include "../../highscore/entry_set.hpp"
#include "settings_fwd.hpp"
#include "../../gui/button.hpp"
#include "../../gui/table/view.hpp"
#include "../../gui/combobox.hpp"
#include "../../events/declare_transition_type.hpp"
#include "../../events/declare_transition_reaction.hpp"
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
namespace menu
{
class highscore
:
	// The second argument has to be a complete type
	public boost::statechart::state<highscore,menu::superstate>,
	public fruitlib::scenic::node<highscore>
{
FCPPT_NONCOPYABLE(
	highscore);
public:
	typedef
	boost::mpl::vector1
	<
		FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::main)
	>
	reactions;

	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	highscore(
		my_context);

	FRUITCUT_APP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::main);

	~highscore();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	gui::button main_menu_button_;
	gui::button quit_button_;
	fcppt::signal::scoped_connection main_menu_button_connection_;
	fcppt::signal::scoped_connection quit_button_connection_;
	app::highscore::provider_sequence providers_;
	gui::combobox source_box_;
	app::highscore::provider::connection_base_ptr connection_;
	fcppt::signal::shared_connection message_connection_;
	fcppt::signal::shared_connection error_connection_;
	fcppt::signal::shared_connection list_connection_;
	app::highscore::get_model table_model_;
	gui::table::view table_view_;

	void
	switch_provider(
		app::highscore::provider::object_base &);

	void
	list_received(
		app::highscore::entry_set const &);

	void
	text_received(
		fcppt::string const &);
};
}
}
}
}

#endif
