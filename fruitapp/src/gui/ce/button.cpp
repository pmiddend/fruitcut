#include <fruitapp/gui/ce/button.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fcppt/text.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <CEGUI/widgets/PushButton.h>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::ce::button::button(
	fruitlib::audio::sound_controller &_sound_controller,
	CEGUI::Window &_window)
:
	sound_controller_(
		_sound_controller),
	push_connection_(
		_window.subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::bind(
					&button::internal_push_callback,
					this,
					std::placeholders::_1)))),
	hover_connection_(
		_window.subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::bind(
					&button::internal_hover_callback,
					this,
					std::placeholders::_1)))),
	push_signal_()
{
}

fcppt::signal::auto_connection
fruitapp::gui::ce::button::push_callback(
	push_callback_function const &f)
{
	return
		push_signal_.connect(
			f);
}

bool
fruitapp::gui::ce::button::internal_push_callback(
	CEGUI::EventArgs const &)
{
	push_signal_();
	sound_controller_.play(
		fruitlib::resource_tree::path(
			FCPPT_TEXT("button_clicked")));
	return
		true;
}

bool
fruitapp::gui::ce::button::internal_hover_callback(
	CEGUI::EventArgs const &)
{
	return
		true;
}
