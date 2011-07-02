#include "settings.hpp"
#include "main.hpp"
#include "../../config_variables.hpp"
#include "../../events/define_transition_reaction.hpp"
#include "../../events/post_transition.hpp"
#include "../../../fruitlib/json/find_and_convert_member.hpp"
#include "../../../fruitlib/resource_tree/path.hpp"
#include "../../../fruitlib/audio/sound_controller.hpp"
#include "../../../media_path.hpp"
#include <sge/systems/instance.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUISlider.h>

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
		*CEGUI::WindowManager::getSingleton().getWindow("SettingsMenu")),
	main_menu_button_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("SettingsMenu/MainMenuButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&settings::main_menu_button_pushed,
					this,
					std::tr1::placeholders::_1)))),
	music_slider_pulled_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("SettingsMenu/MusicVolume")->subscribeEvent(
			CEGUI::Slider::EventValueChanged,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&settings::music_slider_pulled,
					this,
					std::tr1::placeholders::_1)))),
	effects_slider_pulled_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("SettingsMenu/EffectsVolume")->subscribeEvent(
			CEGUI::Slider::EventValueChanged,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&settings::effects_slider_pulled,
					this,
					std::tr1::placeholders::_1)))),
	splatter_slider_pulled_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("SettingsMenu/SplatterFactor")->subscribeEvent(
			CEGUI::Slider::EventValueChanged,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&settings::splatter_slider_pulled,
					this,
					std::tr1::placeholders::_1))))
{
	dynamic_cast<CEGUI::Slider *>(
		CEGUI::WindowManager::getSingleton().getWindow("SettingsMenu/MusicVolume"))->setCurrentValue(
		static_cast<float>(
			context<machine>().config_variables().music_volume().value()));

	dynamic_cast<CEGUI::Slider *>(
		CEGUI::WindowManager::getSingleton().getWindow("SettingsMenu/EffectsVolume"))->setCurrentValue(
		static_cast<float>(
			context<machine>().config_variables().effects_volume().value()));

	dynamic_cast<CEGUI::Slider *>(
		CEGUI::WindowManager::getSingleton().getWindow("SettingsMenu/SplatterFactor"))->setCurrentValue(
		static_cast<float>(
			context<machine>().config_variables().splatter_count_to_area_factor().value()));
}

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::main,
	menu::settings)

fruitcut::app::states::menu::settings::~settings()
{
}

bool
fruitcut::app::states::menu::settings::main_menu_button_pushed(
	CEGUI::EventArgs const &)
{
	FRUITCUT_APP_EVENTS_POST_TRANSITION(
		menu::main);
	return true;
}

bool
fruitcut::app::states::menu::settings::music_slider_pulled(
	CEGUI::EventArgs const &)
{
	context<machine>().config_variables().music_volume().value(
		static_cast<sge::audio::scalar>(
			dynamic_cast<CEGUI::Slider *>(
				CEGUI::WindowManager::getSingleton().getWindow("SettingsMenu/MusicVolume"))->getCurrentValue()));
	return true;
}

bool
fruitcut::app::states::menu::settings::effects_slider_pulled(
	CEGUI::EventArgs const &)
{
	context<machine>().config_variables().effects_volume().value(
		static_cast<sge::audio::scalar>(
			dynamic_cast<CEGUI::Slider *>(
				CEGUI::WindowManager::getSingleton().getWindow("SettingsMenu/EffectsVolume"))->getCurrentValue()));
	return true;
}

bool
fruitcut::app::states::menu::settings::splatter_slider_pulled(
	CEGUI::EventArgs const &)
{
	context<machine>().config_variables().splatter_count_to_area_factor().value(
		static_cast<fruit::area::value_type>(
			dynamic_cast<CEGUI::Slider *>(
				CEGUI::WindowManager::getSingleton().getWindow("SettingsMenu/SplatterFactor"))->getCurrentValue()));
	return true;
}