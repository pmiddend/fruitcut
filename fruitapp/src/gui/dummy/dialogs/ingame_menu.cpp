#include <fruitapp/gui/dummy/dialogs/ingame_menu.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_callback.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::dummy::dialogs::ingame_menu::ingame_menu(
	sge::input::keyboard::device &_keyboard)
:
	quit_(),
	game_(),
	main_menu_(),
	key_connection_(
		_keyboard.key_callback(
			sge::input::keyboard::key_callback{
				std::bind(
					&ingame_menu::key_callback,
					this,
					std::placeholders::_1
				)
			}
		)
	)
{
	std::cout << "Ingame menu, press 'g' to return to game,\n";
	std::cout << "             press 'm' to return to main menu,\n";
	std::cout << "             press 'q' to quit.\n";
}

fcppt::signal::auto_connection
fruitapp::gui::dummy::dialogs::ingame_menu::register_game_callback(
	fruitapp::gui::dialogs::ingame_menu::game_callback const &_f)
{
	return
		game_.connect(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::dummy::dialogs::ingame_menu::register_main_menu_callback(
	fruitapp::gui::dialogs::ingame_menu::main_menu_callback const &_f)
{
	return
		main_menu_.connect(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::dummy::dialogs::ingame_menu::register_quit_callback(
	fruitapp::gui::dialogs::ingame_menu::quit_callback const &_f)
{
	return
		quit_.connect(
			_f);
}

fruitapp::gui::dummy::dialogs::ingame_menu::~ingame_menu()
{
}

void
fruitapp::gui::dummy::dialogs::ingame_menu::key_callback(
	sge::input::keyboard::key_event const &_key_event)
{
	if(!_key_event.pressed())
		return;

	switch(_key_event.key().code())
	{
	case sge::input::key::code::g:
		game_();
		break;
	case sge::input::key::code::q:
		quit_();
		break;
	case sge::input::key::code::m:
		main_menu_();
		break;
	default:
		break;
	}
}
