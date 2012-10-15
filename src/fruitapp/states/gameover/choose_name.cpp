#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/post_transition.hpp>
#include <fruitapp/states/gameover/choose_name.hpp>
#include <fruitapp/states/gameover/ranking.hpp>
#include <fruitlib/media_path.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/insert_to_fcppt_string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <CEGUI/Event.h>
#include <CEGUI/String.h>
#include <CEGUI/Window.h>
#include <CEGUI/WindowManager.h>
#include <fcppt/config/external_end.hpp>


fruitapp::states::gameover::choose_name::choose_name(
	my_context ctx)
:
	my_base(
		ctx),
	layout_(
		context<fruitapp::machine>().gui_system(),
		fruitlib::media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("layouts")/FCPPT_TEXT("name_chooser.layout")),
	gui_sheet_(
		context<fruitapp::machine>().gui_system(),
		layout_.window()),
	continue_button_(
		context<fruitapp::machine>().sound_controller(),
		*layout_.window().getChild(
			"NameChooser/Continue")),
	continue_button_connection_(
		continue_button_.push_callback(
			std::tr1::bind(
				&choose_name::continue_button_pushed,
				this)))
{
	layout_.window().getChild(
		"NameChooser/Score")->setText(
		sge::cegui::to_cegui_string(
		fcppt::insert_to_fcppt_string(
				context<fruitapp::machine>().last_game_score()),
		context<fruitapp::machine>().systems().charconv_system()));
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
		layout_.window().getChild(
			"NameChooser/Name")->getText();

	if(name.empty())
	{
		context<fruitapp::machine>().sound_controller().play(
			fruitlib::resource_tree::path(
				FCPPT_TEXT("name_invalid")));
	}
	else
	{
		context<fruitapp::states::gameover::superstate>().name(
			name);
		FRUITAPP_EVENTS_POST_TRANSITION(
			gameover::ranking);
	}
}
