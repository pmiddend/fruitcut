#ifndef FRUITAPP_GUI_DUMMY_DIALOGS_INGAME_MENU_HPP_INCLUDED
#define FRUITAPP_GUI_DUMMY_DIALOGS_INGAME_MENU_HPP_INCLUDED

#include <fruitapp/gui/dialogs/ingame_menu.hpp>
#include <sge/input/keyboard/device_fwd.hpp>
#include <sge/input/keyboard/key_event_fwd.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/auto_connection.hpp>


namespace fruitapp
{
namespace gui
{
namespace dummy
{
namespace dialogs
{
class ingame_menu
:
	public fruitapp::gui::dialogs::ingame_menu
{
FCPPT_NONCOPYABLE(
	ingame_menu);
public:
	explicit
	ingame_menu(
		sge::input::keyboard::device &);

	fcppt::signal::auto_connection
	register_game_callback(
		fruitapp::gui::dialogs::ingame_menu::game_callback const &);

	fcppt::signal::auto_connection
	register_main_menu_callback(
		fruitapp::gui::dialogs::ingame_menu::main_menu_callback const &);

	fcppt::signal::auto_connection
	register_quit_callback(
		fruitapp::gui::dialogs::ingame_menu::quit_callback const &);

	~ingame_menu();
private:
	fcppt::signal::object<void ()> quit_;
	fcppt::signal::object<void ()> game_;
	fcppt::signal::object<void ()> main_menu_;
	fcppt::signal::auto_connection key_connection_;

	void
	key_callback(
		sge::input::keyboard::key_event const &);
};
}
}
}
}

#endif
