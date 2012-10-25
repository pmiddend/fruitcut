#include <fruitapp/postprocessing.hpp>
#include <fruitapp/quick_log.hpp>
#include <fruitapp/renderer_dim2.hpp>
#include <fruitapp/renderer_rect.hpp>
#include <fruitapp/scene.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/depths/root.hpp>
#include <fruitapp/depths/scene.hpp>
#include <fruitapp/events/define_transition_reaction.hpp>
#include <fruitapp/events/post_transition.hpp>
#include <fruitapp/events/return_post_transition_functor.hpp>
#include <fruitapp/fruit/plane.hpp>
#include <fruitapp/fruit/triangle_traits.hpp>
#include <fruitapp/fruit/hull/projected.hpp>
#include <fruitapp/fruit/hull/ring.hpp>
#include <fruitapp/fruit/hull/trail_intersection.hpp>
#include <fruitapp/states/gameover/choose_name.hpp>
#include <fruitapp/states/gameover/superstate.hpp>
#include <fruitapp/states/ingame/paused.hpp>
#include <fruitapp/states/ingame/running.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/math/multiply_matrix4_vector3.hpp>
#include <fruitlib/math/unproject.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/scenic/depth.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <fruitlib/time_format/find_and_convert_duration.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/first_person/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/image/colors.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/line_drawer/line.hpp>
#include <sge/line_drawer/line_sequence.hpp>
#include <sge/line_drawer/render_to_screen.hpp>
#include <sge/line_drawer/scoped_lock.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/target/onscreen.hpp>
#include <sge/renderer/target/viewport.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/matrix/inverse.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/dot.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/next_prior.hpp>
#include <iostream>
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
		context<fruitapp::machine>().systems().renderer_core()),
	line_drawer_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().overlay_node(),
				fruitlib::scenic::depth(
					depths::overlay::dont_care))),
		line_drawer_,
		fruitlib::scenic::adaptors::line_drawer::optional_renderer(
			context<fruitapp::machine>().systems().renderer_ffp())),
	cursor_sound_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care))),
		context<fruitapp::machine>().systems().cursor_demuxer(),
		context<fruitapp::machine>().ingame_clock(),
		context<fruitapp::machine>().viewport_manager(),
		context<fruitapp::machine>().sound_controller()),
	cursor_trail_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().root_node(),
				fruitlib::scenic::depth(
					depths::root::dont_care))),
		context<fruitapp::machine>().systems().cursor_demuxer(),
		context<fruitapp::machine>().ingame_clock(),
		fruitlib::time_format::find_and_convert_duration<ingame_clock::duration>(
			context<fruitapp::machine>().config_file(),
			sge::parse::json::path(FCPPT_TEXT("mouse"))
					/ FCPPT_TEXT("trail-update-rate")),
		sge::parse::json::find_and_convert_member<fruitapp::cursor_trail::size_type>(
				context<fruitapp::machine>().config_file(),
				sge::parse::json::path(FCPPT_TEXT("mouse"))
					/ FCPPT_TEXT("trail-samples")),
		context<fruitapp::machine>().systems().renderer_core().onscreen_target()),
	fruit_spawned_connection_(
		context<superstate>().fruit_spawner().spawn_callback(
			std::tr1::bind(
				&fruitlib::audio::sound_controller::play,
				&context<fruitapp::machine>().sound_controller(),
				fruitlib::resource_tree::path(
					FCPPT_TEXT("fruit_was_spawned"))))),
	draw_mouse_trail_(
		sge::parse::json::find_and_convert_member<bool>(
			context<fruitapp::machine>().config_file(),
			sge::parse::json::path(FCPPT_TEXT("ingame"))
				/ FCPPT_TEXT("draw-mouse-trail"))),
	draw_bbs_(
		sge::parse::json::find_and_convert_member<bool>(
			context<fruitapp::machine>().config_file(),
			sge::parse::json::path(FCPPT_TEXT("ingame"))
				/ FCPPT_TEXT("draw-bbs"))),
	transit_to_paused_connection_(
		context<fruitapp::machine>().systems().keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				FRUITAPP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
					ingame::paused)))),
	sword_trail_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				context<fruitapp::machine>().scene_node(),
				fruitlib::scenic::depth(
					depths::scene::sword_trail))),
		context<fruitapp::machine>().systems().renderer_ffp(),
		context<fruitapp::machine>().systems().renderer_core().onscreen_target(),
		context<fruitapp::machine>().systems().image_system(),
		context<fruitapp::machine>().systems().cursor_demuxer(),
		context<fruitapp::machine>().ingame_clock(),
		context<fruitapp::machine>().config_file()),
	viewport_change_connection_(
		context<fruitapp::machine>().viewport_manager().change_callback(
			std::tr1::bind(
				&running::viewport_change,
				this,
				std::tr1::placeholders::_1),
			fruitapp::viewport::trigger_early(
				true)))
{
	context<fruitapp::machine>().postprocessing().active(
		true);
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
	if(draw_bbs_ || draw_mouse_trail_)
	{
		sge::line_drawer::scoped_lock slock(
			line_drawer_);
		slock.value().clear();
		if(draw_bbs_)
			draw_fruit_bbs(
				slock.value());
		if(draw_mouse_trail_)
			draw_mouse_trail(
				slock.value());
	}

	for(
		fruitapp::fruit::object_sequence::const_iterator i =
			context<fruitapp::states::ingame::superstate>().fruit_manager().fruits().begin();
		i != context<fruitapp::states::ingame::superstate>().fruit_manager().fruits().end();
		++i)
		this->process_fruit(
			*i);

	if(context<fruitapp::states::ingame::superstate>().game_logic().finished())
	{
		context<fruitapp::machine>().last_game_score(
			context<fruitapp::states::ingame::superstate>().game_logic().score());
		FRUITAPP_EVENTS_POST_TRANSITION(
			gameover::superstate);
	}
}

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
				context<fruitapp::machine>().systems().renderer_core().onscreen_target(),
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

void
fruitapp::states::ingame::running::draw_mouse_trail(
	sge::line_drawer::line_sequence &lines)
{
	std::wcerr << "drawing mouse trail\n";
	if (cursor_trail_.positions().empty())
		return;

	for(
		fruitapp::cursor_trail::position_buffer::const_iterator i =
			cursor_trail_.positions().begin();
		i != boost::prior(cursor_trail_.positions().end());
		++i)
		lines.push_back(
			sge::line_drawer::line(
				sge::renderer::vector3(
					static_cast<sge::renderer::scalar>(
						i->x()),
					static_cast<sge::renderer::scalar>(
						i->y()),
					static_cast<sge::renderer::scalar>(
						0)),
				sge::renderer::vector3(
					static_cast<sge::renderer::scalar>(
						boost::next(i)->x()),
					static_cast<sge::renderer::scalar>(
						boost::next(i)->y()),
					static_cast<sge::renderer::scalar>(
						0)),
				sge::image::colors::red(),
				sge::image::colors::red()));
}

void
fruitapp::states::ingame::running::process_fruit(
	fruitapp::fruit::object const &current_fruit)
{
	sge::renderer::target::base &target(
		context<fruitapp::machine>().systems().renderer_core().onscreen_target());

	fruitapp::fruit::hull::intersection_pair const intersection =
		fruitapp::fruit::hull::trail_intersection(
			fruitapp::fruit::hull::projected(
				current_fruit,
				target,
				sge::camera::matrix_conversion::world_projection(
					context<fruitapp::machine>().camera().coordinate_system(),
					context<fruitapp::machine>().camera().projection_matrix())),
			cursor_trail_.positions());

	if (!intersection)
		return;

	sge::renderer::matrix4 const inverse_mvp =
		fcppt::math::matrix::inverse(
			sge::camera::matrix_conversion::world_projection(
				context<fruitapp::machine>().camera().coordinate_system(),
				context<fruitapp::machine>().camera().projection_matrix()));

	sge::renderer::vector3 const
		// Convert the points to 3D and to renderer::scalar
		point1(
			static_cast<sge::renderer::scalar>(
				intersection->first[0]),
			static_cast<sge::renderer::scalar>(
				intersection->first[1]),
			static_cast<sge::renderer::scalar>(
				0)),
		point2(
			static_cast<sge::renderer::scalar>(
				intersection->second[0]),
			static_cast<sge::renderer::scalar>(
				intersection->second[1]),
			static_cast<sge::renderer::scalar>(
				0)),
		// unproject 'em
		point1_unprojected =
			fruitlib::math::unproject(
				point1,
				inverse_mvp,
				// The points are already "un-viewported", but they are in
				// screenspace, so use the screen rect here
				fruitapp::renderer_rect(
					sge::renderer::vector2::null(),
					fcppt::math::dim::structure_cast<fruitapp::renderer_dim2>(
						target.viewport().get().size()))),
		point2_unprojected =
			fruitlib::math::unproject(
				point2,
				inverse_mvp,
				// The points are already "un-viewported", but they are in
				// screenspace, so use the screen rect here
				fruitapp::renderer_rect(
					sge::renderer::vector2::null(),
					fcppt::math::dim::structure_cast<fruitapp::renderer_dim2>(
						target.viewport().get().size()))),
		point3_unprojected =
			fruitlib::math::unproject(
				sge::renderer::vector3(
					point1.x(),
					point1.y(),
					static_cast<sge::renderer::scalar>(
						0.5)),
				inverse_mvp,
				fruitapp::renderer_rect(
					sge::renderer::vector2::null(),
					fcppt::math::dim::structure_cast<fruitapp::renderer_dim2>(
						target.viewport().get().size()))),
		first_plane_vector =
			point2_unprojected - point1_unprojected,
		second_plane_vector =
			point3_unprojected - point1_unprojected,
		// NOTE: For rotation matrices M and vectors a,b the following holds:
		// cross(M*a,M*b) = M*cross(a,b)
		plane_normal =
			fruitlib::math::multiply_matrix4_vector3(
				fcppt::math::matrix::transpose(
					current_fruit.rotation()),
				fcppt::math::vector::cross(
					first_plane_vector,
					second_plane_vector));

	sge::renderer::scalar const plane_scalar =
		fcppt::math::vector::dot(
			fruitlib::math::multiply_matrix4_vector3(
				fcppt::math::matrix::transpose(
					current_fruit.rotation()),
				point1_unprojected - current_fruit.position()),
			plane_normal);

	context<superstate>().fruit_manager().cut(
		current_fruit,
		fruit::plane(
			plane_normal,
			plane_scalar),
		// cut direction
		first_plane_vector,
		cursor_trail_.total_expiry_duration());

	//cursor_trail_.clear();
}

void
fruitapp::states::ingame::running::viewport_change(
	sge::renderer::target::viewport const &)
{
	cursor_trail_.clear();
}