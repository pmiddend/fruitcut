#include <fruitapp/gui/dummy/dialogs/main_menu.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::dummy::dialogs::main_menu::main_menu(
	sge::input::keyboard::device &_keyboard)
:
	quit_(),
	start_(),
	key_connection_(
		_keyboard.key_callback(
			std::tr1::bind(
				&main_menu::key_callback,
				this,
				std::tr1::placeholders::_1)))
{
	std::cout << "Main menu, press 's' to start,\n";
	std::cout << "           press 'q' to quit.\n";
}

fcppt::signal::auto_connection
fruitapp::gui::dummy::dialogs::main_menu::register_quit_callback(
	fruitapp::gui::dialogs::main_menu::quit_callback const &_quit_callback)
{
	return
		quit_.connect(
			_quit_callback);
}

fcppt::signal::auto_connection
fruitapp::gui::dummy::dialogs::main_menu::register_settings_callback(
	fruitapp::gui::dialogs::main_menu::settings_callback const &)
{
	return
		fcppt::signal::auto_connection();
}

fcppt::signal::auto_connection
fruitapp::gui::dummy::dialogs::main_menu::register_start_callback(
	fruitapp::gui::dialogs::main_menu::start_callback const &_start_callback)
{
	return
		start_.connect(
			_start_callback);
}

fcppt::signal::auto_connection
fruitapp::gui::dummy::dialogs::main_menu::register_highscore_callback(
	fruitapp::gui::dialogs::main_menu::highscore_callback const &)
{
	return
		fcppt::signal::auto_connection();
}

fruitapp::gui::dummy::dialogs::main_menu::~main_menu()
{
}

void
fruitapp::gui::dummy::dialogs::main_menu::key_callback(
	sge::input::keyboard::key_event const &_key_event)
{
	if(!_key_event.pressed())
		return;

	switch(_key_event.key_code())
	{
	case sge::input::keyboard::key_code::s:
		start_();
		break;
	case sge::input::keyboard::key_code::q:
		quit_();
		break;
	default:
		break;
	}
}