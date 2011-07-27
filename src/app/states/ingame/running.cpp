#include "running.hpp"
#include "paused.hpp"
#include "../../quick_log.hpp"
#include "../gameover/superstate.hpp"
#include "../../postprocessing.hpp"
#include "../gameover/choose_name.hpp"
#include "../../dim2.hpp"
#include "../../scene.hpp"
#include "../../fruit/plane.hpp"
#include "../../fruit/triangle_traits.hpp"
#include "../../fruit/hull/trail_intersection.hpp"
#include "../../fruit/hull/projected.hpp"
#include "../../fruit/hull/ring.hpp"
#include "../../events/define_transition_reaction.hpp"
#include "../../events/return_post_transition_functor.hpp"
#include "../../events/post_transition.hpp"
#include "../../../fruitlib/json/find_and_convert_member.hpp"
#include "../../../fruitlib/math/multiply_matrix4_vector3.hpp"
#include "../../../fruitlib/math/unproject.hpp"
#include "../../../fruitlib/resource_tree/path.hpp"
#include "../../../fruitlib/audio/sound_controller.hpp"
#include <sge/font/pos.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/text/text.hpp>
#include <sge/font/unit.hpp>
#include <sge/image/colors.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/line_drawer/render_to_screen.hpp>
#include <sge/line_drawer/scoped_lock.hpp>
#include <sge/camera/object.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/systems/instance.hpp>
#include <sge/time/millisecond.hpp>
#include <sge/time/second.hpp>
#include <sge/time/unit.hpp>
#include <sge/viewport/manager.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/text.hpp>
#include <fcppt/ref.hpp>
#include <boost/next_prior.hpp>
#include <iostream>

#include <sge/renderer/state/state.hpp>

fruitcut::app::states::ingame::running::running(
	my_context ctx)
:
	my_base(
		ctx),
	line_drawer_(
		context<machine>().systems().renderer()),
	line_drawer_node_(
		line_drawer_,
		&(context<machine>().systems().renderer())),
	cursor_trail_(
		context<machine>().systems().cursor_demuxer(),
		sge::time::millisecond(
			fruitlib::json::find_and_convert_member<sge::time::unit>(
				context<machine>().config_file(),
				fruitlib::json::path(FCPPT_TEXT("mouse"))
					/ FCPPT_TEXT("trail-update-rate-ms"))),
		context<machine>().timer_callback(),
		fruitlib::json::find_and_convert_member<app::cursor_trail::size_type>(
				context<machine>().config_file(),
				fruitlib::json::path(FCPPT_TEXT("mouse"))
					/ FCPPT_TEXT("trail-samples")),
		context<machine>().systems().renderer().onscreen_target()),
	update_node_(
		std::tr1::bind(
			&running::update,
			this),
		fruitlib::scenic::nodes::intrusive_with_callbacks::render_callback()),
	viewport_change_connection_(
		context<machine>().systems().viewport_manager().manage_callback(
			std::tr1::bind(
				&running::viewport_change,
				this))),
	fruit_spawned_connection_(
		context<superstate>().fruit_spawner().spawn_callback(
			std::tr1::bind(
				&fruitlib::audio::sound_controller::play,
				&context<machine>().sound_controller(),
				fruitlib::resource_tree::path(
					FCPPT_TEXT("fruit_was_spawned"))))),
	draw_mouse_trail_(
		fruitlib::json::find_and_convert_member<bool>(
			context<machine>().config_file(),
			fruitlib::json::path(FCPPT_TEXT("ingame"))
				/ FCPPT_TEXT("draw-mouse-trail"))),
	draw_bbs_(
		fruitlib::json::find_and_convert_member<bool>(
			context<machine>().config_file(),
			fruitlib::json::path(FCPPT_TEXT("ingame"))
				/ FCPPT_TEXT("draw-bbs"))),
	transit_to_paused_connection_(
		context<machine>().systems().keyboard_collector().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape, 
				FRUITCUT_APP_EVENTS_RETURN_POST_TRANSITION_FUNCTOR(
					ingame::paused)))),
	sword_trail_(
		context<app::machine>().systems().renderer(),
		context<app::machine>().systems().renderer().onscreen_target(),
		context<app::machine>().systems().image_loader(),
		context<app::machine>().systems().cursor_demuxer(),
		context<app::machine>().timer_callback(),
		context<app::machine>().config_file())
{
	context<machine>().scene_node().push_back(
		sword_trail_);
	context<machine>().overlay_node().insert_dont_care(
		update_node_);
	context<machine>().overlay_node().insert_dont_care(
		line_drawer_node_);
	context<machine>().overlay_node().insert_dont_care(
		cursor_trail_);
	context<machine>().postprocessing().active(
		true);
	viewport_change();
}

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	ingame::paused,
	ingame::running)

FRUITCUT_APP_EVENTS_DEFINE_TRANSITION_REACTION(
	gameover::superstate,
	ingame::running)

fruitcut::app::states::ingame::running::~running()
{
}

void
fruitcut::app::states::ingame::running::update()
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
		fruit::object_sequence::const_iterator i = 
			context<superstate>().fruit_manager().fruits().begin(); 
		i != context<superstate>().fruit_manager().fruits().end(); 
		++i)
		process_fruit(
			*i);

	if(context<superstate>().game_logic().finished())
	{
		context<machine>().last_game_score(
			context<superstate>().game_logic().score());
		FRUITCUT_APP_EVENTS_POST_TRANSITION(
			gameover::superstate);
	}
}

void
fruitcut::app::states::ingame::running::draw_fruit_bbs(
	sge::line_drawer::line_sequence &lines)
{
	for(
		fruit::object_sequence::const_iterator i = 
			context<superstate>().fruit_manager().fruits().begin(); 
		i != context<superstate>().fruit_manager().fruits().end();
		++i) 
	{
		fruit::hull::ring const hull = 
			fruit::hull::projected(
				*i,
				context<machine>().systems().renderer().onscreen_target(),
				context<machine>().camera().mvp());

		for(
			fruit::hull::ring::const_iterator hull_point = hull.begin(); 
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
fruitcut::app::states::ingame::running::draw_mouse_trail(
	sge::line_drawer::line_sequence &lines)
{
	std::wcerr << "drawing mouse trail\n";
	if (cursor_trail_.positions().empty())
		return;

	for(
		app::cursor_trail::position_buffer::const_iterator i = 
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
fruitcut::app::states::ingame::running::process_fruit(
	fruit::object const &current_fruit)
{
	fruit::hull::intersection_pair const intersection = 
		fruit::hull::trail_intersection(
			fruit::hull::projected(
				current_fruit,
				context<machine>().systems().renderer().onscreen_target(),
				context<machine>().camera().mvp()),
			cursor_trail_.positions());

	if (!intersection)
		return;

	sge::renderer::matrix4 const inverse_mvp =
		fcppt::math::matrix::inverse(
			context<machine>().camera().mvp());

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
				fcppt::math::box::basic<sge::renderer::scalar,2>(
					sge::renderer::vector2::null(),
					fcppt::math::dim::structure_cast<dim2>(
						context<machine>().systems().renderer().onscreen_target().viewport().get().size()))),
		point2_unprojected = 
			fruitlib::math::unproject(
				point2,
				inverse_mvp,
				// The points are already "un-viewported", but they are in
				// screenspace, so use the screen rect here
				fcppt::math::box::basic<sge::renderer::scalar,2>(
					sge::renderer::vector2::null(),
					fcppt::math::dim::structure_cast<dim2>(
						context<machine>().systems().renderer().onscreen_target().viewport().get().size()))),
		point3_unprojected = 
			fruitlib::math::unproject(
				sge::renderer::vector3(
					point1.x(),
					point1.y(),
					static_cast<sge::renderer::scalar>(
						0.5)),
				inverse_mvp,
				fcppt::math::box::basic<sge::renderer::scalar,2>(
					sge::renderer::vector2::null(),
					fcppt::math::dim::structure_cast<dim2>(
						context<machine>().systems().renderer().onscreen_target().viewport().get().size()))),
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
		cursor_trail_.total_expiry_duration(),
		context<machine>().timer_callback());

	//cursor_trail_.clear();
}

void
fruitcut::app::states::ingame::running::viewport_change()
{
	cursor_trail_.clear();
}
