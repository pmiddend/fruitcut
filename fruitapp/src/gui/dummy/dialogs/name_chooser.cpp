#include <fruitapp/gui/dummy/dialogs/name_chooser.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::dummy::dialogs::name_chooser::name_chooser(
	sge::input::keyboard::device &_keyboard)
:
	continue_(),
	key_connection_(
		_keyboard.key_callback(
			std::tr1::bind(
				&name_chooser::key_callback,
				this,
				std::tr1::placeholders::_1)))
{
	std::cout << "Name chooser, press any key to continue\n";
}

fcppt::signal::auto_connection
fruitapp::gui::dummy::dialogs::name_chooser::register_continue_callback(
	fruitapp::gui::dialogs::name_chooser::continue_callback const &_f)
{
	return
		continue_.connect(
			_f);
}

fcppt::string const
fruitapp::gui::dummy::dialogs::name_chooser::name() const
{
	return
		FCPPT_TEXT("anonymous");
}

fruitapp::gui::dummy::dialogs::name_chooser::~name_chooser()
{
}

void
fruitapp::gui::dummy::dialogs::name_chooser::key_callback(
	sge::input::keyboard::key_event const &_key_event)
{
	if(!_key_event.pressed())
		return;

	continue_();
}
