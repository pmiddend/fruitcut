#include <fruitapp/gui/ce/system.hpp>
#include <fruitapp/gui/ce/dialogs/settings.hpp>
#include <fruitlib/media_path.hpp>


fruitapp::gui::ce::dialogs::settings::settings(
	fruitapp::gui::ce::system &_system,
	fruitapp::gui::initial_effects_volume const &_initial_effects_volume,
	fruitapp::gui::initial_music_volume const &_initial_music_volume)
:
	gui_node_(
		fruitlib::scenic::optional_parent(
			_system.overlay_node()),
		_system.gui_system(),
		_system.standard_clock_callback()),
	gui_keyboard_(
		_system.gui_syringe(),
		_system.keyboard()),
	gui_cursor_(
		_system.gui_syringe(),
		_system.cursor()),
	layout_(
		_system.gui_system(),
		fruitlib::media_path()
			/ FCPPT_TEXT("gui")
			/ FCPPT_TEXT("layouts")
			/ FCPPT_TEXT("settings_menu.layout")),
	gui_sheet_(
		_system.gui_system(),
		*layout_.window().getChild("SettingsMenu")),
	main_menu_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"SettingsMenu/Return")),
	music_volume_slider_(
		_system.sound_controller(),
		layout_.window(),
		"SettingsMenu/MusicVolume",
		static_cast<fruitapp::gui::ce::progress_slider::value_type>(
			_initial_music_volume.get().get()) / 100.0f),
	music_volume_connection_(
		music_volume_slider_.value_changed(
			std::tr1::bind(
				&settings::music_volume_callback,
				this,
				std::tr1::placeholders::_1))),
	music_volume_change_(),
	effects_volume_slider_(
		_system.sound_controller(),
		layout_.window(),
		"SettingsMenu/EffectsVolume",
		static_cast<fruitapp::gui::ce::progress_slider::value_type>(
			_initial_effects_volume.get().get()) / 100.0f),
	effects_volume_connection_(
		effects_volume_slider_.value_changed(
			std::tr1::bind(
				&settings::effects_volume_callback,
				this,
				std::tr1::placeholders::_1))),
	effects_volume_change_()
{
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::settings::register_effects_volume_change_callback(
	fruitapp::gui::dialogs::settings::volume_change_callback const &_f)
{
	return
		effects_volume_change_.connect(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::settings::register_music_volume_change_callback(
	fruitapp::gui::dialogs::settings::volume_change_callback const &_f)
{
	return
		music_volume_change_.connect(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::settings::register_back_callback(
	fruitapp::gui::dialogs::settings::back_callback const &_f)
{
	return
		main_menu_button_.push_callback(
			_f);
}

fruitapp::gui::ce::dialogs::settings::~settings()
{
}

void
fruitapp::gui::ce::dialogs::settings::music_volume_callback(
	fruitapp::gui::ce::progress_slider::value_type const _value)
{
	music_volume_change_(
		fruitapp::gui::sound_volume(
			static_cast<fruitapp::gui::sound_volume::value_type>(
				_value * 100.0f)));
}

void
fruitapp::gui::ce::dialogs::settings::effects_volume_callback(
	fruitapp::gui::ce::progress_slider::value_type const _value)
{
	effects_volume_change_(
		fruitapp::gui::sound_volume(
			static_cast<fruitapp::gui::sound_volume::value_type>(
				_value * 100.0f)));
}
