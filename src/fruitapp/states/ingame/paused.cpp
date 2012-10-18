#include <fruitapp/postprocessing.hpp>
#include <fruitapp/scene.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/depths/root.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/generic_transition.hpp>
#include <fruitapp/events/return_post_transition_functor.hpp>
#include <fruitapp/gui/system.hpp>
#include <fruitapp/gui/dialogs/ingame_menu.hpp>
#include <fruitapp/states/ingame/paused.hpp>
#include <fruitapp/states/ingame/running.hpp>
#include <fruitapp/states/menu/main.hpp>
#include <fruitlib/media_path.hpp>
#include <fruitlib/pp/filter/blur.hpp>
#include <fruitlib/pp/texture/use_screen_size.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <fruitlib/time_format/find_and_convert_duration.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/systems/instance.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <sge/timer/clocks/standard.hpp>
#include <awl/main/exit_success.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/tr1/functional.hpp>


fruitapp::states::ingame::paused::paused(
	my_context ctx)
:
	my_base(
		ctx),
	node_base(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().overlay_node(),
				fruitlib::scenic::depth(
					depths::overlay::paused)))),
	time_factor_(
		context<fruitapp::machine>(),
		0.0f),
	scene_deactivation_(
		context<fruitapp::machine>().scene_node(),
		false),
	pp_deactivation_(
		context<fruitapp::machine>().postprocessing(),
		false),
	system_(
		context<fruitapp::machine>().postprocessing().filter_manager()),
	inject_texture_(
		context<fruitapp::machine>().postprocessing().texture_manager()),
	blur_(
		context<fruitapp::machine>().postprocessing().filter_manager(),
		context<fruitapp::machine>().postprocessing().texture_manager(),
		fruitlib::pp::filter::texture_size(
			fruitlib::pp::texture::use_screen_size()),
		fruitlib::pp::filter::iterations(
			1u)),
	current_texture_(
		context<fruitapp::machine>().postprocessing().result_texture()),
	blur_iterations_(
		0u),
	max_blur_iterations_(
		sge::parse::json::find_and_convert_member<fruitlib::pp::filter::iterations::value_type>(
			context<fruitapp::machine>().config_file(),
			sge::parse::json::path(FCPPT_TEXT("paused"))
				/ FCPPT_TEXT("max-blur-iterations"))),
	blur_timer_(
		sge::timer::parameters<sge::timer::clocks::standard>(
			fruitlib::time_format::find_and_convert_duration<sge::timer::clocks::standard::duration>(
				context<fruitapp::machine>().config_file(),
				sge::parse::json::path(FCPPT_TEXT("paused"))
					/ FCPPT_TEXT("blur-frequency-time")))),
	transit_to_running_connection_(
		context<fruitapp::machine>().systems().keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				std::tr1::bind(
					// Note that using post_event does something unexpected. If
					// you use that, you get a tick event first and _then_ the
					// toggle_pause event, which is not the desired behaviour
					// (post_event posts to the queue, process_event immediately
					// processes it)
					&fruitapp::machine::post_event,
					&context<fruitapp::machine>(),
					events::generic_transition<ingame::running>())))),
	ingame_menu_(
		context<fruitapp::machine>().gui_system().create_ingame_menu()),
	continue_connection_(
		ingame_menu_->register_game_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				ingame::running))),
	main_menu_connection_(
		ingame_menu_->register_main_menu_callback(
			FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
				menu::main))),
	quit_connection_(
		ingame_menu_->register_quit_callback(
			std::tr1::bind(
				&fruitapp::machine::quit,
				&context<fruitapp::machine>(),
				awl::main::exit_success())))
{
	system_.add_filter(
		inject_texture_,
		FCPPT_TEXT("inject_texture"),
		fruitlib::pp::dependency_set());

	system_.add_filter(
		blur_,
		FCPPT_TEXT("blur"),
		fcppt::assign::make_container<fruitlib::pp::dependency_set>
			(FCPPT_TEXT("inject_texture")));
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

void
fruitapp::states::ingame::paused::react(
	fruitlib::scenic::events::render const &_render_event)
{
	system_.render_result(
		_render_event.context());
}

void
fruitapp::states::ingame::paused::react(
	fruitlib::scenic::events::update const &)
{
	if(!blur_iterations_.get() || (blur_iterations_ < max_blur_iterations_ && sge::timer::reset_when_expired(blur_timer_)))
	{
		inject_texture_.texture(
			current_texture_);
		system_.update();
		current_texture_ =
			system_.result_texture();
		blur_iterations_++;
	}
}

