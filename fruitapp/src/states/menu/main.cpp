#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/return_post_transition_functor.hpp>
#include <fruitapp/gui/system.hpp>
#include <fruitapp/gui/dialogs/main_menu.hpp>
#include <fruitapp/states/ingame/running.hpp>
#include <fruitapp/states/menu/highscore.hpp>
#include <fruitapp/states/menu/main.hpp>
#include <fruitapp/states/menu/settings.hpp>
#include <fruitlib/scenic/depth.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <sge/systems/instance.hpp>
#include <awl/main/exit_success.hpp>
#include <fcppt/text.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::states::menu::main::main(
	my_context ctx)
:
	my_base(
		ctx),
	logo_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().overlay_node(),
				fruitlib::scenic::depth(
					depths::overlay::dont_care))),
		context<fruitapp::machine>().systems().renderer_ffp(),
		context<fruitapp::machine>().texture_manager(),
		context<fruitapp::machine>().viewport_manager()),
	main_menu_(
		context<fruitapp::machine>().gui_system().create_main_menu()),
	settings_button_connection_(
		main_menu_->register_settings_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::settings))),
	highscore_button_connection_(
		main_menu_->register_highscore_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::highscore))),
	quit_button_connection_(
		main_menu_->register_quit_callback(
			std::bind(
				&fruitapp::machine::quit,
				&context<fruitapp::machine>(),
				awl::main::exit_success()))),
	start_button_connection_(
		main_menu_->register_start_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				ingame::superstate)))
{
}

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	ingame::superstate,
	menu::main)

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::settings,
	menu::main)

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::highscore,
	menu::main)

fruitapp::states::menu::main::~main()
{
}
