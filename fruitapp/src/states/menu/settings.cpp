#include <fruitapp/config_variables/object.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/post_transition.hpp>
#include <fruitapp/events/return_post_transition_functor.hpp>
#include <fruitapp/gui/system.hpp>
#include <fruitapp/gui/dialogs/settings.hpp>
#include <fruitapp/states/menu/main.hpp>
#include <fruitapp/states/menu/settings.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/text.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::states::menu::settings::settings(
	my_context ctx)
:
	my_base(
		ctx),
	settings_(
		this->context<fruitapp::machine>().gui_system().create_settings(
			fruitapp::gui::initial_effects_volume(
				fruitapp::gui::sound_volume(
					static_cast<fruitapp::gui::sound_volume::value_type>(
						this->context<fruitapp::machine>().config_variables().effects_volume().value() * 100.0f))),
			fruitapp::gui::initial_music_volume(
				fruitapp::gui::sound_volume(
					static_cast<fruitapp::gui::sound_volume::value_type>(
						this->context<fruitapp::machine>().config_variables().music_volume().value() * 100.0f))),
			this->context<fruitapp::machine>().graphics_settings())),
	main_menu_button_connection_(
		settings_->register_back_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main))),
	music_volume_connection_(
		settings_->register_music_volume_change_callback(
			std::bind(
				&settings::music_volume_callback,
				this,
				std::placeholders::_1))),
	effects_volume_connection_(
		settings_->register_effects_volume_change_callback(
			std::bind(
				&settings::effects_volume_callback,
				this,
				std::placeholders::_1))),
	quality_connection_(
		settings_->register_quality_change_callback(
			std::bind(
				&settings::quality_callback,
				this,
				std::placeholders::_1)))
{
}

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::main,
	menu::settings)

fruitapp::states::menu::settings::~settings()
{
}

void
fruitapp::states::menu::settings::music_volume_callback(
	fruitapp::gui::sound_volume const &v)
{
	this->context<fruitapp::machine>().config_variables().music_volume().value(
		static_cast<sge::audio::scalar>(
			v.get()) / 100.0f);
}

void
fruitapp::states::menu::settings::effects_volume_callback(
	fruitapp::gui::sound_volume const &v)
{
	this->context<fruitapp::machine>().config_variables().effects_volume().value(
		static_cast<sge::audio::scalar>(
			v.get()) / 100.0f);
}

void
fruitapp::states::menu::settings::quality_callback(
	fruitapp::graphics_settings::preset_identifier const &v)
{
	this->context<fruitapp::machine>().config_variables().graphics_preset().value(
		v.get());
}
