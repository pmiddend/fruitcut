#include <fruitapp/quick_log.hpp>
#include <fruitapp/cursor/manager.hpp>
#include <fruitapp/cursor/instance.hpp>
#include <fruitapp/cursor/instance_sequence.hpp>
#include <fruitapp/scene.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/depths/root.hpp>
#include <fruitapp/depths/scene.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/post_transition.hpp>
#include <fruitapp/events/return_post_transition_functor.hpp>
//#include <fruitapp/fruit/hull/projected.hpp>
//#include <fruitapp/fruit/hull/ring.hpp>
#include <fruitapp/states/gameover/choose_name.hpp>
#include <fruitapp/states/gameover/superstate.hpp>
#include <fruitapp/states/ingame/paused.hpp>
#include <fruitapp/states/ingame/running.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/scenic/depth.hpp>
#include <fruitlib/scenic/parent.hpp>
//#include <sge/camera/coordinate_system/object.hpp>
//#include <sge/camera/first_person/object.hpp>
//#include <sge/camera/matrix_conversion/world_projection.hpp>
//#include <sge/image/colors.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/action_callback.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_code.hpp>
//#include <sge/line_drawer/line.hpp>
#include <sge/line_drawer/line_sequence.hpp>
#include <sge/line_drawer/scoped_lock.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/target/onscreen.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/text.hpp>
#include <fcppt/config/external_begin.hpp>
//#include <boost/next_prior.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::states::ingame::running::running(
	my_context ctx)
:
	my_base(
		ctx),
	node_base(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care)))),
	line_drawer_(
		context<fruitapp::machine>().systems().renderer_device_core()),
	line_drawer_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().overlay_node(),
				fruitlib::scenic::depth(
					depths::overlay::dont_care))),
		line_drawer_,
		fruitlib::scenic::adaptors::line_drawer::optional_renderer(
			context<fruitapp::machine>().systems().renderer_device_ffp())),
	cursor_sequence_(
		context<fruitapp::machine>().cursor_manager().create_instances(
			fruitlib::scenic::optional_parent(
				fruitlib::scenic::parent(
					context<fruitapp::machine>().scene_node(),
					fruitlib::scenic::depth(
						depths::scene::sword_trail))))),
	fruit_spawned_connection_(
		context<superstate>().fruit_spawner().spawn_callback(
			fruitapp::fruit::spawner::spawn_callback_function{
				std::bind(
					&fruitlib::audio::sound_controller::play,
					&context<fruitapp::machine>().sound_controller(),
					fruitlib::resource_tree::path(
						FCPPT_TEXT("fruit_was_spawned")
					)
				)
			}
		)
	),
	draw_mouse_trail_(
		sge::parse::json::find_and_convert_member<bool>(
			context<fruitapp::machine>().config_file(),
			sge::parse::json::path(FCPPT_TEXT("ingame"))
				/ FCPPT_TEXT("draw-mouse-trail"))),
/*
	draw_bbs_(
		sge::parse::json::find_and_convert_member<bool>(
			context<fruitapp::machine>().config_file(),
			sge::parse::json::path(FCPPT_TEXT("ingame"))
				/ FCPPT_TEXT("draw-bbs"))),
*/
	transit_to_paused_connection_(
		context<fruitapp::machine>().systems().keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				sge::input::keyboard::action_callback{
					FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
						ingame::paused
					)
				}
			)
		)
	)
{
}

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	ingame::paused,
	ingame::running)

FRUITAPP_EVENTS_DEFINE_TRANSITION_REACTION(
	gameover::superstate,
	ingame::running)

fruitapp::states::ingame::running::~running()
{
}

void
fruitapp::states::ingame::running::react(
	fruitlib::scenic::events::update const &)
{
	if(/*draw_bbs_ || */draw_mouse_trail_)
	{
		sge::line_drawer::scoped_lock slock(
			line_drawer_);
		slock.value().clear();
		/*
		if(draw_bbs_)
			draw_fruit_bbs(
				slock.value());
		*/
		if(draw_mouse_trail_)
			cursor_sequence_->draw_trails(
				slock.value());
	}

	for(
		auto const &fruit
		:
		context<fruitapp::states::ingame::superstate>().fruit_manager().fruits()
	)
		this->process_fruit(
			*fruit);

	if(context<fruitapp::states::ingame::superstate>().game_logic().finished())
	{
		context<fruitapp::machine>().last_game_score(
			context<fruitapp::states::ingame::superstate>().game_logic().score());
		FRUITAPP_EVENTS_POST_TRANSITION(
			gameover::superstate);
	}
}

/*
void
fruitapp::states::ingame::running::draw_fruit_bbs(
	sge::line_drawer::line_sequence &lines)
{
	for(
		fruitapp::fruit::object_sequence::const_iterator i =
			context<fruitapp::states::ingame::superstate>().fruit_manager().fruits().begin();
		i != context<fruitapp::states::ingame::superstate>().fruit_manager().fruits().end();
		++i)
	{
		fruitapp::fruit::hull::ring const hull =
			fruitapp::fruit::hull::projected(
				*i,
				context<fruitapp::machine>().systems().renderer_device_core().onscreen_target(),
				sge::camera::matrix_conversion::world_projection(
					context<fruitapp::machine>().camera().coordinate_system(),
					context<fruitapp::machine>().camera().projection_matrix()));

		for(
			fruitapp::fruit::hull::ring::const_iterator hull_point = hull.begin();
			hull_point != boost::prior(hull.end());
			++hull_point)
		{
			lines.push_back(
				sge::line_drawer::line(
					sge::renderer::vector3(
						static_cast<sge::renderer::scalar>(
							hull_point->x()),
						static_cast<sge::renderer::scalar>(
							hull_point->y()),
						static_cast<sge::renderer::scalar>(
							0)),
					sge::renderer::vector3(
						static_cast<sge::renderer::scalar>(
							boost::next(hull_point)->x()),
						static_cast<sge::renderer::scalar>(
							boost::next(hull_point)->y()),
						static_cast<sge::renderer::scalar>(
							0)),
					sge::image::colors::green(),
					sge::image::colors::green()));
		}
	}
}
*/

void
fruitapp::states::ingame::running::process_fruit(
	fruitapp::fruit::object const &_current_fruit)
{
	for(
		auto const &current_cursor
		:
		cursor_sequence_->cursors()
	)
		this->process_fruit(
			_current_fruit,
			*current_cursor);
}

void
fruitapp::states::ingame::running::process_fruit(
	fruitapp::fruit::object const &_current_fruit,
	fruitapp::cursor::instance const &_current_cursor)
{
	context<fruitapp::states::ingame::superstate>().fruit_manager().cut(
		_current_fruit,
		_current_cursor.calculate_fruit_intersection(
			_current_fruit),
		fruitapp::fruit::ban_duration(
			_current_cursor.total_expiry_duration()),
		context<fruitapp::machine>().systems().renderer_device_core().onscreen_target());
}
