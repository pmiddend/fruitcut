#include <fruitapp/scene.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/depths/root.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/generic_transition.hpp>
#include <fruitapp/events/return_post_transition_functor.hpp>
#include <fruitapp/gui/system.hpp>
#include <fruitapp/gui/dialogs/ingame_menu.hpp>
#include <fruitapp/postprocessing/system.hpp>
#include <fruitapp/postprocessing/subsystems/paused.hpp>
#include <fruitapp/states/ingame/paused.hpp>
#include <fruitapp/states/ingame/running.hpp>
#include <fruitapp/states/menu/main.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <sge/input/key/action_callback.hpp>
#include <sge/input/key/code.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/systems/instance.hpp>
#include <awl/main/exit_success.hpp>
#include <fcppt/text.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::states::ingame::paused::paused(
	my_context ctx)
:
	my_base(
		ctx),
	time_factor_(
		context<fruitapp::machine>(),
		0.0f),
/*
	scene_deactivation_(
		context<fruitapp::machine>().scene_node(),
		false),
*/
	paused_postprocessing_(
		context<fruitapp::machine>().postprocessing_system().create_paused_subsystem(
			fruitlib::scenic::optional_parent(
				fruitlib::scenic::parent(
					context<fruitapp::machine>().overlay_node(),
					fruitlib::scenic::depth(
						depths::overlay::paused))))),
	transit_to_running_connection_(
		context<fruitapp::machine>().systems().keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::key::code::escape,
				sge::input::key::action_callback{
					std::bind(
						// Note that using post_event does something unexpected. If
						// you use that, you get a tick event first and _then_ the
						// toggle_pause event, which is not the desired behaviour
						// (post_event posts to the queue, process_event immediately
						// processes it)
						&fruitapp::machine::post_event,
						&context<fruitapp::machine>(),
						events::generic_transition<ingame::running>()
					)
				}
			)
		)
	),
	ingame_menu_(
		context<fruitapp::machine>().gui_system().create_ingame_menu()),
	continue_connection_(
		ingame_menu_->register_game_callback(
			fruitapp::gui::dialogs::ingame_menu::game_callback{
				FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
					ingame::running
				)
			}
		)
	),
	main_menu_connection_(
		ingame_menu_->register_main_menu_callback(
			fruitapp::gui::dialogs::ingame_menu::main_menu_callback{
				FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
					menu::main
				)
			}
		)
	),
	quit_connection_(
		ingame_menu_->register_quit_callback(
			fruitapp::gui::dialogs::ingame_menu::quit_callback{
				std::bind(
					&fruitapp::machine::quit,
					&context<fruitapp::machine>(),
					awl::main::exit_success()
				)
			}
		)
	)
{
}

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	ingame::running,
	ingame::paused)

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	menu::main,
	ingame::paused)

fruitapp::states::ingame::paused::~paused()
{
}
