#include "running.hpp"
#include "paused.hpp"
#include "../gameover/superstate.hpp"
#include "../../postprocessing.hpp"
#include "../gameover/choose_name.hpp"
#include "../../dim2.hpp"
#include "../../fruit/plane.hpp"
#include "../../fruit/triangle_traits.hpp"
#include "../../fruit/hull/trail_intersection.hpp"
#include "../../fruit/hull/projected.hpp"
#include "../../fruit/hull/ring.hpp"
#include "../../events/generic_transition.hpp"
#include "../../../fruitlib/json/find_and_convert_member.hpp"
#include "../../../fruitlib/math/multiply_matrix4_vector3.hpp"
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
#include <sge/line_drawer/render_to_screen.hpp>
#include <sge/line_drawer/scoped_lock.hpp>
#include <sge/camera/object.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/state/state.hpp>
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

fruitcut::app::states::ingame::running::running(
	my_context ctx)
:
	my_base(
		ctx),
	scoped_render_state_(
		context<machine>().systems().renderer(),
		sge::renderer::state::list
			(sge::renderer::state::depth_func::less)
			(sge::renderer::state::cull_mode::off)
			(sge::renderer::state::bool_::clear_depth_buffer = true)
			(sge::renderer::state::float_::depth_buffer_clear_val = 1.0f)),
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
		fruitlib::json::find_and_convert_member<fruitlib::cursor_trail::size_type>(
				context<machine>().config_file(),
				fruitlib::json::path(FCPPT_TEXT("mouse"))
					/ FCPPT_TEXT("trail-samples")),
		context<machine>().systems().renderer().onscreen_target()),
	cursor_trail_node_(
		cursor_trail_),
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
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::p, 
				std::tr1::bind(
					// Note that using post_event does something unexpected. If
					// you use that, you get a tick event first and _then_ the
					// toggle_pause event, which is not the desired behaviour
					// (post_event posts to the queue, process_event immediately
					// processes it)
					&machine::post_event,
					&context<machine>(),
					events::generic_transition<ingame::paused>()))))
{
	context<machine>().overlay_node().insert_dont_care(
		update_node_);
	context<machine>().overlay_node().insert_dont_care(
		line_drawer_node_);
	context<machine>().overlay_node().insert_dont_care(
		cursor_trail_node_);
	context<machine>().postprocessing().active(
		true);
	viewport_change();
}

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
			app::score(
				context<superstate>().game_logic().score()));
		context<machine>().post_event(
			events::generic_transition<states::gameover::superstate>());
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
	if (cursor_trail_.positions().empty())
		return;

	for(
		fruitlib::cursor_trail::position_buffer::const_iterator i = 
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
			fcppt::math::matrix::unproject(
				point1,
				inverse_mvp,
				// The points are already "un-viewported", but they are in
				// screenspace, so use the screen rect here
				fcppt::math::box::basic<sge::renderer::scalar,2>(
					sge::renderer::vector2::null(),
					fcppt::math::dim::structure_cast<dim2>(
						context<machine>().systems().renderer().onscreen_target().viewport().get().size()))),
		point2_unprojected = 
			fcppt::math::matrix::unproject(
				point2,
				inverse_mvp,
				// The points are already "un-viewported", but they are in
				// screenspace, so use the screen rect here
				fcppt::math::box::basic<sge::renderer::scalar,2>(
					sge::renderer::vector2::null(),
					fcppt::math::dim::structure_cast<dim2>(
						context<machine>().systems().renderer().onscreen_target().viewport().get().size()))),
		point3_unprojected = 
			fcppt::math::matrix::unproject(
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
		cursor_trail_.expiry_duration(),
		context<machine>().timer_callback());

	//cursor_trail_.clear();
}

void
fruitcut::app::states::ingame::running::viewport_change()
{
	cursor_trail_.clear();
}
