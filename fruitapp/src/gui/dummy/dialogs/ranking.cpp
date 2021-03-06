#include <fruitapp/gui/dummy/dialogs/ranking.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_callback.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::dummy::dialogs::ranking::ranking(
	sge::input::keyboard::device &_keyboard)
:
	highscore_(),
	main_menu_(),
	key_connection_(
		_keyboard.key_callback(
			sge::input::keyboard::key_callback{
				std::bind(
					&ranking::key_callback,
					this,
					std::placeholders::_1
				)
			}
		)
	)
{
	std::cout << "In ranking menu, press 'h' to return to highscore,\n";
	std::cout << "                 press 'm' to return to main menu.\n";
}

fcppt::signal::auto_connection
fruitapp::gui::dummy::dialogs::ranking::register_highscore_callback(
	fruitapp::gui::dialogs::ranking::highscore_callback const &_f)
{
	return
		highscore_.connect(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::dummy::dialogs::ranking::register_main_menu_callback(
	fruitapp::gui::dialogs::ranking::main_menu_callback const &_f)
{
	return
		main_menu_.connect(
			_f);
}

void
fruitapp::gui::dummy::dialogs::ranking::append_log(
	fcppt::string const &)
{
}

void
fruitapp::gui::dummy::dialogs::ranking::post(
	fruitapp::highscore::name const &,
	fruitapp::highscore::score const &)
{
}

fruitapp::gui::dummy::dialogs::ranking::~ranking()
{
}

void
fruitapp::gui::dummy::dialogs::ranking::key_callback(
	sge::input::keyboard::key_event const &_key_event)
{
	if(!_key_event.pressed())
		return;

	switch(_key_event.key().code())
	{
	case sge::input::key::code::h:
		highscore_();
		break;
	case sge::input::key::code::m:
		main_menu_();
		break;
	default:
		break;
	}
}
