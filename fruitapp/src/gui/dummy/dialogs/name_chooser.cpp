#include <fruitapp/gui/dummy/dialogs/name_chooser.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <fcppt/text.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::dummy::dialogs::name_chooser::name_chooser(
	sge::input::keyboard::device &_keyboard)
:
	submit_(),
	main_menu_(),
	restart_(),
	key_connection_(
		_keyboard.key_callback(
			std::bind(
				&name_chooser::key_callback,
				this,
				std::placeholders::_1)))
{
	std::cout << "Main menu, press 's' to submit name,\n";
	std::cout << "           press 'm' to go to the main menu.\n";
	std::cout << "           press 'r' to restart.\n";
}

fcppt::signal::auto_connection
fruitapp::gui::dummy::dialogs::name_chooser::register_submit_callback(
	fruitapp::gui::dialogs::name_chooser::submit_callback const &_f)
{
	return
		submit_.connect(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::dummy::dialogs::name_chooser::register_main_menu_callback(
	fruitapp::gui::dialogs::name_chooser::main_menu_callback const &_f)
{
	return
		main_menu_.connect(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::dummy::dialogs::name_chooser::register_restart_callback(
	fruitapp::gui::dialogs::name_chooser::restart_callback const &_f)
{
	return
		restart_.connect(
			_f);
}

fcppt::string const
fruitapp::gui::dummy::dialogs::name_chooser::name() const
{
	return
		FCPPT_TEXT("anonymous");
}

void
fruitapp::gui::dummy::dialogs::name_chooser::name(
	fcppt::string const &)
{
}

fruitapp::gui::dummy::dialogs::name_chooser::~name_chooser()
{
}

void
fruitapp::gui::dummy::dialogs::name_chooser::key_callback(
	sge::input::keyboard::key_event const &_key_event)
{
	switch(_key_event.key().code())
	{
	case sge::input::keyboard::key_code::s:
		submit_();
		break;
	case sge::input::keyboard::key_code::m:
		main_menu_();
		break;
	case sge::input::keyboard::key_code::r:
		restart_();
		break;
	default:
		break;
	}
}
