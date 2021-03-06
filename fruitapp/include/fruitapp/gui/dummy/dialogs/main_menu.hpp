#ifndef FRUITAPP_GUI_DUMMY_DIALOGS_MAIN_MENU_HPP_INCLUDED
#define FRUITAPP_GUI_DUMMY_DIALOGS_MAIN_MENU_HPP_INCLUDED

#include <fruitapp/gui/dialogs/main_menu.hpp>
#include <sge/input/keyboard/device_fwd.hpp>
#include <sge/input/keyboard/key_event_fwd.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>


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
		fruitapp::gui::dialogs::main_menu::quit_callback const &
	)
	override;

	fcppt::signal::auto_connection
	register_settings_callback(
		fruitapp::gui::dialogs::main_menu::settings_callback const &
	)
	override;

	fcppt::signal::auto_connection
	register_start_callback(
		fruitapp::gui::dialogs::main_menu::start_callback const &
	)
	override;

	fcppt::signal::auto_connection
	register_highscore_callback(
		fruitapp::gui::dialogs::main_menu::highscore_callback const &
	)
	override;

	~main_menu()
	override;
private:
	fcppt::signal::object<void ()> quit_;
	fcppt::signal::object<void ()> settings_;
	fcppt::signal::object<void ()> start_;
	fcppt::signal::object<void ()> highscore_;
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
