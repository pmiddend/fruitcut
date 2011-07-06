#include "settings.hpp"
#include "main.hpp"
#include "../../config_variables.hpp"
#include "../../events/define_transition_reaction.hpp"
#include "../../events/return_post_transition_functor.hpp"
#include "../../events/post_transition.hpp"
#include "../../../fruitlib/json/find_and_convert_member.hpp"
#include "../../../fruitlib/resource_tree/path.hpp"
#include "../../../fruitlib/audio/sound_controller.hpp"
#include "../../../media_path.hpp"
#include <sge/systems/instance.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/cegui/system.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <CEGUIWindowManager.h>

fruitcut::app::states::menu::settings::settings(
	my_context ctx)
:
	my_base(
		ctx),
	layout_(
		fruitcut::media_path()
			/FCPPT_TEXT("gui")
			/FCPPT_TEXT("layouts")
			/FCPPT_TEXT("settings_menu.layout"),
		context<machine>().systems().charconv_system()),
	gui_sheet_(
		*context<app::machine>().gui_system().window_manager().getWindow("SettingsMenu")),
	main_menu_button_(
		context<machine>().sound_controller(),
		*context<app::machine>().gui_system().window_manager().getWindow(
			"SettingsMenu/Return")),
	main_menu_button_connection_(
		main_menu_button_.push_callback(
			FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main))),
	music_volume_slider_(
		context<app::machine>().sound_controller(),
		context<app::machine>().gui_system().window_manager(),
		"SettingsMenu/MusicVolume",
		context<machine>().config_variables().music_volume().value()),
	music_volume_connection_(
		music_volume_slider_.value_changed(
			std::tr1::bind(
				&settings::music_volume_callback,
				this,
				std::tr1::placeholders::_1))),
	effects_volume_slider_(
		context<app::machine>().sound_controller(),
		context<app::machine>().gui_system().window_manager(),
		"SettingsMenu/EffectsVolume",
		context<machine>().config_variables().effects_volume().value()),
	effects_volume_connection_(
		effects_volume_slider_.value_changed(
			std::tr1::bind(
				&settings::effects_volume_callback,
				this,
				std::tr1::placeholders::_1))),
	splatter_slider_(
		context<app::machine>().sound_controller(),
		context<app::machine>().gui_system().window_manager(),
		"SettingsMenu/ParticleDensity",
		context<machine>().config_variables().splatter_count_to_area_factor().value()),
	splatter_connection_(
		music_volume_slider_.value_changed(
			std::tr1::bind(
				&settings::particle_callback,
				this,
				std::tr1::placeholders::_1)))
{
}

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::main,
	menu::settings)

fruitcut::app::states::menu::settings::~settings()
{
}

void
fruitcut::app::states::menu::settings::music_volume_callback(
	gui::progress_slider::value_type const v)
{
	context<machine>().config_variables().music_volume().value(
		static_cast<sge::audio::scalar>(
			v));
}

void
fruitcut::app::states::menu::settings::effects_volume_callback(
	gui::progress_slider::value_type const v)
{
	context<machine>().config_variables().effects_volume().value(
		static_cast<sge::audio::scalar>(
			v));
}

void
fruitcut::app::states::menu::settings::particle_callback(
	gui::progress_slider::value_type const v)
{
	context<machine>().config_variables().splatter_count_to_area_factor().value(
		static_cast<fruit::area::value_type>(
			v));
}
