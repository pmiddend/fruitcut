#include <fruitapp/gui/ce/button.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fcppt/text.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <CEGUI/widgets/PushButton.h>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::ce::button::button(
	fruitlib::audio::sound_controller &_sound_controller,
	CEGUI::Window &_window)
:
	sound_controller_(
		_sound_controller
	),
	push_connection_(
		_window.subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				[
					this
				](
					CEGUI::EventArgs const &
				)
				-> bool
				{
					push_signal_();

					sound_controller_.play(
						fruitlib::resource_tree::path(
							FCPPT_TEXT("button_clicked")
						)
					);

					return
						true;
				}
			)
		)
	),
	hover_connection_(
		_window.subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				[](
					CEGUI::EventArgs const &
				)
				-> bool
				{
					return
						true;
				}
			)
		)
	),
	push_signal_()
{
}

fruitapp::gui::ce::button::~button()
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
