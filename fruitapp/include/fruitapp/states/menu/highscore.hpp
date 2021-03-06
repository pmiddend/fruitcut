#ifndef FRUITAPP_STATES_MENU_HIGHSCORE_HPP_INCLUDED
#define FRUITAPP_STATES_MENU_HIGHSCORE_HPP_INCLUDED

#include <fruitapp/events/declare_transition_reaction.hpp>
#include <fruitapp/events/declare_transition_type.hpp>
#include <fruitapp/gui/dialogs/highscore_unique_ptr.hpp>
#include <fruitapp/highscore/entry_set.hpp>
#include <fruitapp/highscore/provider_sequence.hpp>
#include <fruitapp/highscore/provider/connection_base_ptr.hpp>
#include <fruitapp/highscore/provider/object_base.hpp>
#include <fruitapp/states/menu/settings_fwd.hpp>
#include <fruitapp/states/menu/superstate.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/parent_fwd.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fcppt/string.hpp>
#include <fcppt/optional/object_decl.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/statechart/state.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
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
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			menu::main)
	>
	reactions;

	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	highscore(
		my_context);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		menu::main);

	~highscore();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	typedef
	fcppt::optional::object<
		fcppt::signal::auto_connection
	>
	optional_connection;

	fruitapp::highscore::provider_sequence providers_;
	fruitapp::gui::dialogs::highscore_unique_ptr highscore_;
	optional_connection main_menu_button_connection_;
	optional_connection switch_provider_connection_;
	typedef
	fcppt::optional::object<
		fruitapp::highscore::provider::connection_base_ptr
	>
	optional_provider_connection;
	optional_provider_connection connection_;
	optional_connection message_connection_;
	optional_connection error_connection_;
	optional_connection list_connection_;

	void
	switch_provider(
		fruitapp::highscore::provider::object_base &);

	void
	list_received(
		fruitapp::highscore::entry_set const &);

	void
	text_received(
		fcppt::string const &);
};
}
}
}

#endif
