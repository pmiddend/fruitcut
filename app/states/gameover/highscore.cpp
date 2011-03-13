#include "highscore.hpp"
#include "../intro.hpp"
#include "../../events/gameover/continue_button_pushed.hpp"
#include "../../../media_path.hpp"
#include <sge/cegui/to_cegui_string.hpp>
#include <CEGUI/CEGUIEvent.h>
#include <CEGUI/CEGUIString.h>
#include <CEGUI/CEGUIWindow.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/elements/CEGUIPushButton.h>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/string.hpp>
#include <boost/lexical_cast.hpp>

fruitcut::app::states::gameover::highscore::highscore(
	my_context ctx)
:
	my_base(
		ctx),
	layout_(
		media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("layouts")/FCPPT_TEXT("highscore.layout"),
		context<machine>().systems().charconv_system()),
	quit_button_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("Highscore/QuitButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&highscore::quit_button_pushed,
					this,
					std::tr1::placeholders::_1)))),
	reset_button_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("Highscore/ResetButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&highscore::reset_button_pushed,
					this,
					std::tr1::placeholders::_1))))
{
}

boost::statechart::result
fruitcut::app::states::gameover::highscore::react(
	events::gameover::quit_button_pushed const &)
{
	context<machine>().quit();
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::gameover::highscore::react(
	events::gameover::reset_button_pushed const &)
{
	return transit<intro>();
}

fruitcut::app::states::gameover::highscore::~highscore()
{
}

bool
fruitcut::app::states::gameover::highscore::quit_button_pushed(
	CEGUI::EventArgs const &)
{
	context<machine>().sound_controller().play(
		FCPPT_TEXT("button-clicked"));
	post_event(
		events::gameover::quit_button_pushed());
	return true;
}

bool
fruitcut::app::states::gameover::highscore::reset_button_pushed(
	CEGUI::EventArgs const &)
{
	context<machine>().sound_controller().play(
		FCPPT_TEXT("button-clicked"));
	post_event(
		events::gameover::reset_button_pushed());
	return true;
}
