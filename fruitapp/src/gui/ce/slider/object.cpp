#include <fruitapp/gui/ce/slider/object.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fcppt/text.hpp>
#include <fcppt/config/external_begin.hpp>
#include <CEGUI/Window.h>
#include <CEGUI/widgets/Slider.h>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::ce::slider::object::object(
	fruitlib::audio::sound_controller &_sound_controller,
	CEGUI::Window &_window,
	fruitapp::gui::ce::slider::range const &_range,
	fruitapp::gui::ce::slider::current_value const &_current_value)
:
	sound_controller_(
		_sound_controller),
	value_changed_signal_(),
	value_changed_connection_(
		_window.subscribeEvent(
			CEGUI::Slider::EventValueChanged,
			CEGUI::Event::Subscriber(
				[
					this
				](
					CEGUI::EventArgs const &_args
				)
				-> bool
				{
					value_changed_signal_(
						static_cast<CEGUI::Slider &>(
							*static_cast<CEGUI::WindowEventArgs const &>(
								_args).window).getCurrentValue() +
						range_.min());
					return
						true;
				}
			)
		)
	),
	value_change_complete_connection_(
		_window.subscribeEvent(
			CEGUI::Slider::EventThumbTrackEnded,
			CEGUI::Event::Subscriber(
				[
					this
				](
					CEGUI::EventArgs const &
				)
				-> bool
				{
					sound_controller_.play(
						fruitlib::resource_tree::path(
							FCPPT_TEXT("button_clicked")));
					return
						true;
				}
			)
		)
	),
	range_(
		_range)
{
	static_cast<CEGUI::Slider &>(_window).setMaxValue(
		static_cast<float>(
			range_.max() - range_.min()));

	static_cast<CEGUI::Slider &>(_window).setCurrentValue(
		static_cast<float>(
			_current_value.get() - range_.min()));
}

fcppt::signal::auto_connection
fruitapp::gui::ce::slider::object::value_changed(
	fruitapp::gui::ce::slider::value_changed_function const &_f)
{
	return
		value_changed_signal_.connect(
			_f);
}

fruitapp::gui::ce::slider::object::~object()
{
}
