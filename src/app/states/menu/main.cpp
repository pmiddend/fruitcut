#include "main.hpp"
#include "settings.hpp"
#include "../../config_variables.hpp"
#include "../ingame/running.hpp"
#include "../../events/define_transition_reaction.hpp"
#include "../../events/post_transition.hpp"
#include "../../../fruitlib/json/find_and_convert_member.hpp"
#include "../../../fruitlib/audio/sound_controller.hpp"
#include "../../../media_path.hpp"
#include <sge/systems/instance.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUISlider.h>

fruitcut::app::states::menu::main::main(
	my_context ctx)
:
	my_base(
		ctx),
	logo_(
		context<machine>().systems().renderer(),
		context<machine>().systems().viewport_manager(),
		context<machine>().systems().image_loader(),
		context<machine>().config_file()),
	layout_(
		fruitcut::media_path()
			/FCPPT_TEXT("gui")
			/FCPPT_TEXT("layouts")
			/FCPPT_TEXT("main_menu.layout"),
		context<machine>().systems().charconv_system()),
	gui_sheet_(
		*CEGUI::WindowManager::getSingleton().getWindow("MainMenu")),
	quit_button_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("MainMenu/Quit")->subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&main::quit_button_pushed,
					this,
					std::tr1::placeholders::_1)))),
	start_button_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("MainMenu/StartGame")->subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&main::start_button_pushed,
					this,
					std::tr1::placeholders::_1)))),
	settings_button_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("MainMenu/Settings")->subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&main::settings_button_pushed,
					this,
					std::tr1::placeholders::_1))))
{
	context<machine>().overlay_node().insert_dont_care(
		logo_);
}

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	ingame::superstate,
	menu::main)

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::settings,
	menu::main)

fruitcut::app::states::menu::main::~main()
{
}

bool
fruitcut::app::states::menu::main::quit_button_pushed(
	CEGUI::EventArgs const &)
{
	context<machine>().quit();
	return true;
}

bool
fruitcut::app::states::menu::main::start_button_pushed(
	CEGUI::EventArgs const &)
{
	FRUITCUT_APP_EVENTS_POST_TRANSITION(
		ingame::superstate);
	return true;
}

bool
fruitcut::app::states::menu::main::settings_button_pushed(
	CEGUI::EventArgs const &)
{
	FRUITCUT_APP_EVENTS_POST_TRANSITION(
		menu::settings);
	return true;
}
