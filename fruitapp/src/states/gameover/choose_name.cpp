#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/post_transition.hpp>
#include <fruitapp/gui/system.hpp>
#include <fruitapp/gui/dialogs/name_chooser.hpp>
#include <fruitapp/states/gameover/choose_name.hpp>
#include <fruitapp/states/gameover/ranking.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/insert_to_fcppt_string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>


fruitapp::states::gameover::choose_name::choose_name(
	my_context ctx)
:
	my_base(
		ctx),
	name_chooser_(
		context<fruitapp::machine>().gui_system().create_name_chooser(
			fruitapp::highscore::score(
				context<fruitapp::machine>().last_game_score()))),
	continue_button_connection_(
		name_chooser_->register_continue_callback(
			std::tr1::bind(
				&choose_name::continue_button_pushed,
				this)))
{
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
