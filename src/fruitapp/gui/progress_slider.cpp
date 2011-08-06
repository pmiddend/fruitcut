#include <fruitapp/gui/progress_slider.hpp>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIProgressBar.h>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <algorithm>

namespace
{
fruitapp::gui::progress_slider::value_type const increase_amount = 
	static_cast<fruitapp::gui::progress_slider::value_type>(
		0.1);
}

fruitapp::gui::progress_slider::progress_slider(
	fruitlib::audio::sound_controller &_sound_controller,
	CEGUI::WindowManager &_window_manager,
	std::string const &prefix,
	value_type const initial_value)
:
	value_changed_signal_(),
	progress_window_(
		dynamic_cast<CEGUI::ProgressBar &>(
			*_window_manager.getWindow(
				prefix+"Progress"))),
	increase_(
		_sound_controller,
		*_window_manager.getWindow(
			prefix+"Increase")),
	decrease_(
		_sound_controller,
		*_window_manager.getWindow(
			prefix+"Decrease")),
	increase_connection_(
		increase_.push_callback(
			std::tr1::bind(
				&progress_slider::increase_callback,
				this))),
	decrease_connection_(
		decrease_.push_callback(
			std::tr1::bind(
				&progress_slider::decrease_callback,
				this)))
{
	progress_window_.setProgress(
		initial_value);
}

fcppt::signal::auto_connection
fruitapp::gui::progress_slider::value_changed(
	value_changed_function const &f)
{
	return 
		value_changed_signal_.connect(
			f);
}

fruitapp::gui::progress_slider::~progress_slider()
{
}

void
fruitapp::gui::progress_slider::increase_callback()
{
	progress_window_.setProgress(
		std::min(
			static_cast<value_type>(
				1),
			progress_window_.getProgress() + increase_amount));
	value_changed_signal_(
		progress_window_.getProgress());
}

void
fruitapp::gui::progress_slider::decrease_callback()
{
	progress_window_.setProgress(
		std::max(
			static_cast<value_type>(
				0),
			progress_window_.getProgress() - increase_amount));
	value_changed_signal_(
		progress_window_.getProgress());
}
