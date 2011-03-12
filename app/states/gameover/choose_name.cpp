#include "choose_name.hpp"
#include "highscore.hpp"
#include "../../events/gameover/continue_button_pushed.hpp"
#include "../../../media_path.hpp"
#include <sge/cegui/to_cegui_string.hpp>
#include <CEGUI/CEGUIEvent.h>
#include <CEGUI/CEGUIString.h>
#include <CEGUI/CEGUIWindow.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/elements/CEGUIPushButton.h>
#include <boost/lexical_cast.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/tr1/functional.hpp>

fruitcut::app::states::gameover::choose_name::choose_name(
	my_context ctx)
:
	my_base(
		ctx),
	layout_(
		media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("layouts")/FCPPT_TEXT("name_chooser.layout"),
		context<machine>().systems().charconv_system()),
	continue_button_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("NameChooser/ContinueButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&choose_name::continue_button_pushed,
					this,
					std::tr1::placeholders::_1))))
{
	CEGUI::WindowManager::getSingleton().getWindow("NameChooser/Congratulations")->setText(
		sge::cegui::to_cegui_string(
			FCPPT_TEXT("Congratulations!\nYour score:\n")+
			boost::lexical_cast<fcppt::string>(
				context<machine>().last_game_score()),
			context<machine>().systems().charconv_system()));
}

boost::statechart::result
fruitcut::app::states::gameover::choose_name::react(
	events::gameover::continue_button_pushed const &)
{
	return transit<highscore>();
}

fruitcut::app::states::gameover::choose_name::~choose_name()
{
}

bool
fruitcut::app::states::gameover::choose_name::continue_button_pushed(
	CEGUI::EventArgs const &)
{
	CEGUI::String const name = 
		CEGUI::WindowManager::getSingleton().getWindow("NameChooser/NameBox")->getText();
	if(name.empty())
		return true;
	context<superstate>().name(
		name);
	post_event(
		events::gameover::continue_button_pushed());
	return true;
}
