#include <fruitapp/config_variables/object.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/post_transition.hpp>
#include <fruitapp/events/return_post_transition_functor.hpp>
#include <fruitapp/gui/system.hpp>
#include <fruitapp/gui/dialogs/name_chooser.hpp>
#include <fruitapp/states/gameover/choose_name.hpp>
#include <fruitapp/states/gameover/ranking.hpp>
#include <fruitapp/states/ingame/running.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <sge/config/user_name.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/insert_to_fcppt_string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::states::gameover::choose_name::choose_name(
	my_context ctx)
:
	my_base(
		ctx),
	name_chooser_(
		context<fruitapp::machine>().gui_system().create_name_chooser(
			fruitapp::highscore::score(
				context<fruitapp::machine>().last_game_score()))),
	submit_button_connection_(
		name_chooser_->register_submit_callback(
			std::bind(
				&choose_name::submit_button_pushed,
				this))),
	main_menu_button_connection_(
		name_chooser_->register_main_menu_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main))),
	restart_button_connection_(
		name_chooser_->register_restart_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				ingame::superstate)))
{
	if(context<fruitapp::machine>().config_variables().last_user_name().value() == FCPPT_TEXT("$SYSTEM_USER_NAME"))
		name_chooser_->name(
			sge::config::user_name());
	else
		name_chooser_->name(
			context<fruitapp::machine>().config_variables().last_user_name().value());
}

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	gameover::ranking,
	gameover::choose_name)

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	ingame::superstate,
	gameover::choose_name)

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::main,
	gameover::choose_name)

fruitapp::states::gameover::choose_name::~choose_name()
{
}

void
fruitapp::states::gameover::choose_name::submit_button_pushed()
{
	if(name_chooser_->name().empty())
	{
		context<fruitapp::machine>().sound_controller().play(
			fruitlib::resource_tree::path(
				FCPPT_TEXT("name_invalid")));
	}
	else
	{
		context<fruitapp::states::gameover::superstate>().name(
			name_chooser_->name());

		FRUITAPP_EVENTS_POST_TRANSITION(
			gameover::ranking);
	}
}
