#include <fruitapp/states/gameover/choose_name.hpp>
#include <fruitapp/states/gameover/ranking.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/post_transition.hpp>
#include "../../../media_path.hpp"
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <sge/cegui/system.hpp>
#include <sge/systems/instance.hpp>
#include <CEGUIEvent.h>
#include <CEGUIString.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <fcppt/insert_to_fcppt_string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>

fruitapp::states::gameover::choose_name::choose_name(
	my_context ctx)
:
	my_base(
		ctx),
	layout_(
		context<machine>().gui_system(),
		fruitcut::media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("layouts")/FCPPT_TEXT("name_chooser.layout"),
		context<machine>().systems().charconv_system()),
	gui_sheet_(
		context<machine>().gui_system(),
		*context<machine>().gui_system().window_manager().getWindow("NameChooser")),
	continue_button_(
		context<machine>().sound_controller(),
		*context<machine>().gui_system().window_manager().getWindow(
			"NameChooser/Continue")),
	continue_button_connection_(
		continue_button_.push_callback(
			std::tr1::bind(
				&choose_name::continue_button_pushed,
				this)))
{
	context<machine>().gui_system().window_manager().getWindow("NameChooser/Score")->setText(
		sge::cegui::to_cegui_string(
		fcppt::insert_to_fcppt_string(
				context<machine>().last_game_score()),
		context<machine>().systems().charconv_system()));
}

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	gameover::ranking,
	gameover::choose_name)

fruitapp::states::gameover::choose_name::~choose_name()
{
}

void
fruitapp::states::gameover::choose_name::continue_button_pushed()
{
	CEGUI::String const name =
		context<machine>().gui_system().window_manager().getWindow("NameChooser/Name")->getText();

	if(name.empty())
	{
		context<machine>().sound_controller().play(
			fruitlib::resource_tree::path(
				FCPPT_TEXT("name_invalid")));
	}
	else
	{
		context<superstate>().name(
			name);
		FRUITAPP_EVENTS_POST_TRANSITION(
			gameover::ranking);
	}
}
