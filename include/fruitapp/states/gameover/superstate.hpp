#ifndef FRUITAPP_STATES_GAMEOVER_SUPERSTATE_HPP_INCLUDED
#define FRUITAPP_STATES_GAMEOVER_SUPERSTATE_HPP_INCLUDED

#include <fruitapp/machine.hpp>
#include <fruitapp/states/gameover/choose_name_fwd.hpp>
#include <fruitlib/scenic/adaptors/gui_system.hpp>
#include <sge/cegui/default_cursor.hpp>
#include <sge/cegui/default_keyboard.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/statechart/state.hpp>
#include <CEGUI/String.h>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace states
{
namespace gameover
{
class superstate
:
	// The second argument has to be a complete type
	public boost::statechart::state<superstate,fruitapp::machine,fruitapp::states::gameover::choose_name>
{
FCPPT_NONCOPYABLE(
	superstate);
public:
	explicit
	superstate(
		my_context);

	void
	name(
		CEGUI::String const &);

	CEGUI::String const &
	name() const;

	virtual ~superstate();
private:
	fruitlib::scenic::adaptors::gui_system gui_node_;
	sge::cegui::default_keyboard gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
	CEGUI::String name_;
};
}
}
}

#endif
