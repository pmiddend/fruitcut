#ifndef FRUITAPP_GUI_DUMMY_DIALOGS_MAIN_MENU_HPP_INCLUDED
#define FRUITAPP_GUI_DUMMY_DIALOGS_MAIN_MENU_HPP_INCLUDED

#include <fruitapp/gui/dialogs/main_menu.hpp>
#include <sge/input/keyboard/device_fwd.hpp>
#include <sge/input/keyboard/key_event_fwd.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/scoped_connection.hpp>


namespace fruitapp
{
namespace gui
{
namespace dummy
{
namespace dialogs
{
class main_menu
:
	public fruitapp::gui::dialogs::main_menu
{
FCPPT_NONCOPYABLE(
	main_menu);
public:
	explicit
	main_menu(
		sge::input::keyboard::device &);

	fcppt::signal::auto_connection
	register_quit_callback(
		fruitapp::gui::dialogs::main_menu::quit_callback const &);

	fcppt::signal::auto_connection
	register_settings_callback(
		fruitapp::gui::dialogs::main_menu::settings_callback const &);

	fcppt::signal::auto_connection
	register_start_callback(
		fruitapp::gui::dialogs::main_menu::start_callback const &);

	fcppt::signal::auto_connection
	register_highscore_callback(
		fruitapp::gui::dialogs::main_menu::highscore_callback const &);

	~main_menu();
private:
	fcppt::signal::object<void ()> quit_;
	fcppt::signal::object<void ()> start_;
	fcppt::signal::scoped_connection key_connection_;

	void
	key_callback(
		sge::input::keyboard::key_event const &);
};
}
}
}
}

#endif
