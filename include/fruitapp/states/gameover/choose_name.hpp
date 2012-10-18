#ifndef FRUITAPP_STATES_GAMEOVER_CHOOSE_NAME_HPP_INCLUDED
#define FRUITAPP_STATES_GAMEOVER_CHOOSE_NAME_HPP_INCLUDED

#include <fruitapp/events/declare_transition_reaction.hpp>
#include <fruitapp/events/declare_transition_type.hpp>
#include <fruitapp/gui/dialogs/name_chooser_unique_ptr.hpp>
#include <fruitapp/states/gameover/ranking_fwd.hpp>
#include <fruitapp/states/gameover/superstate.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/statechart/state.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace states
{
namespace gameover
{
class choose_name
:
	// The second argument has to be a complete type
	public boost::statechart::state<choose_name,fruitapp::states::gameover::superstate>
{
FCPPT_NONCOPYABLE(
	choose_name);
public:
	typedef
	boost::mpl::vector1
	<
		FRUITAPP_EVENTS_DECLARE_TRANSITION_TYPE(
			fruitapp::states::gameover::ranking)
	>
	reactions;

	explicit
	choose_name(
		my_context);

	FRUITAPP_EVENTS_DECLARE_TRANSITION_REACTION(
		gameover::ranking);

	virtual ~choose_name();
private:
	fruitapp::gui::dialogs::name_chooser_unique_ptr name_chooser_;
	fcppt::signal::scoped_connection continue_button_connection_;

	void
	continue_button_pushed();
};
}
}
}

#endif
