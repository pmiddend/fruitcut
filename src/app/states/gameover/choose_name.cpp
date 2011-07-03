#include "choose_name.hpp"
#include "highscore.hpp"
#include "../../events/define_transition_reaction.hpp"
#include "../../events/post_transition.hpp"
#include "../../../media_path.hpp"
#include "../../../fruitlib/resource_tree/path.hpp"
#include "../../../fruitlib/audio/sound_controller.hpp"
#include <sge/cegui/to_cegui_string.hpp>
#include <sge/systems/instance.hpp>
#include <CEGUIEvent.h>
#include <CEGUIString.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>
#include <fcppt/lexical_cast.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/tr1/functional.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/core.hpp>
#include <boost/spirit/home/phoenix/statement.hpp>
#include <iostream>

fruitcut::app::states::gameover::choose_name::choose_name(
	my_context ctx)
:
	my_base(
		ctx),
	layout_(
		media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("layouts")/FCPPT_TEXT("name_chooser.layout"),
		context<machine>().systems().charconv_system()),
	gui_sheet_(
		*CEGUI::WindowManager::getSingleton().getWindow("NameChooser")),
	continue_button_connection_(
		CEGUI::WindowManager::getSingleton().getWindow("NameChooser/Continue")->subscribeEvent(
			CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(
				std::tr1::bind(
					&choose_name::continue_button_pushed,
					this,
					std::tr1::placeholders::_1))))
{
	CEGUI::WindowManager::getSingleton().getWindow("NameChooser/Score")->setText(
		sge::cegui::to_cegui_string(
		fcppt::lexical_cast<fcppt::string>(
				context<machine>().last_game_score()),
		context<machine>().systems().charconv_system()));
}

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	gameover::highscore,
	gameover::choose_name)

fruitcut::app::states::gameover::choose_name::~choose_name()
{
}

bool
fruitcut::app::states::gameover::choose_name::continue_button_pushed(
	CEGUI::EventArgs const &)
{
	CEGUI::String const name = 
		CEGUI::WindowManager::getSingleton().getWindow("NameChooser/Name")->getText();
	if(name.empty())
	{
		context<machine>().sound_controller().play(
			fruitlib::resource_tree::path(
				FCPPT_TEXT("name_invalid")));
		return true;
	}

	context<machine>().sound_controller().play(
		fruitlib::resource_tree::path(
			FCPPT_TEXT("button_clicked")));
	context<superstate>().name(
		name);
	FRUITCUT_APP_EVENTS_POST_TRANSITION(
		gameover::highscore);
	return true;
}
