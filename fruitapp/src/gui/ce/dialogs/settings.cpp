#include <fruitapp/media_path.hpp>
#include <sge/parse/json/config/user_config_variable_impl.hpp>
#include <fruitapp/graphics_settings/object.hpp>
#include <fruitapp/gui/ce/system.hpp>
#include <fruitapp/gui/ce/dialogs/settings.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/phoenix/core/reference.hpp>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::ce::dialogs::settings::settings(
	fruitapp::gui::ce::system &_system,
	fruitapp::gui::initial_effects_volume const &_initial_effects_volume,
	fruitapp::gui::initial_music_volume const &_initial_music_volume,
	fruitapp::graphics_settings::object &_graphics_settings)
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
		fruitapp::media_path()
			/ FCPPT_TEXT("gui")
			/ FCPPT_TEXT("layouts")
			/ FCPPT_TEXT("settings_menu.layout")),
	gui_sheet_(
		_system.gui_system(),
		layout_.window()),
	main_menu_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"Return")),
	music_volume_slider_(
		_system.sound_controller(),
		*(layout_.window().getChild(
			  "MusicVolume")),
		fruitapp::gui::ce::slider::range(
			0.0f,
			100.0f),
		fruitapp::gui::ce::slider::current_value(
			static_cast<fruitapp::gui::ce::slider::scalar>(
				_initial_music_volume.get().get()))),
	music_volume_connection_(
		music_volume_slider_.value_changed(
			std::tr1::bind(
				&settings::music_volume_callback,
				this,
				std::tr1::placeholders::_1))),
	music_volume_change_(),
	effects_volume_slider_(
		_system.sound_controller(),
		*(layout_.window().getChild(
			  "EffectsVolume")),
		fruitapp::gui::ce::slider::range(
			0.0f,
			100.0f),
		fruitapp::gui::ce::slider::current_value(
			static_cast<fruitapp::gui::ce::slider::scalar>(
				_initial_effects_volume.get().get()))),
	effects_volume_connection_(
		effects_volume_slider_.value_changed(
			std::tr1::bind(
				&settings::effects_volume_callback,
				this,
				std::tr1::placeholders::_1))),
	effects_volume_change_(),
	quality_change_(),
	quality_dropdown_(
		_system.charconv_system(),
		*layout_.window().getChild(
			"QualityDropDown"))
{
	fruitapp::graphics_settings::preset_name_set const preset_names(
		_graphics_settings.preset_names());

	for(
		fruitapp::graphics_settings::preset_name_set::const_iterator it =
			preset_names.begin();
		it != preset_names.end();
		++it)
		quality_dropdown_.add(
			it->get(),
			boost::phoenix::bind(
				boost::phoenix::ref(
					quality_change_),
				*it),
			fruitapp::gui::ce::combobox::selected(
				it->get() == _graphics_settings.current_preset().value()));
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
fruitapp::gui::ce::dialogs::settings::register_quality_change_callback(
	fruitapp::gui::dialogs::settings::quality_change_callback const &_f)
{
	return
		quality_change_.connect(
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
	fruitapp::gui::ce::slider::scalar const _value)
{
	music_volume_change_(
		fruitapp::gui::sound_volume(
			static_cast<fruitapp::gui::sound_volume::value_type>(
				_value)));
}

void
fruitapp::gui::ce::dialogs::settings::effects_volume_callback(
	fruitapp::gui::ce::slider::scalar const _value)
{
	effects_volume_change_(
		fruitapp::gui::sound_volume(
			static_cast<fruitapp::gui::sound_volume::value_type>(
				_value)));
}
