#include "running.hpp"
#include "paused.hpp"
#include "gameover/superstate.hpp"
#include "gameover/choose_name.hpp"
#include "../dim2.hpp"
#include "../line_drawer/scoped_lock.hpp"
#include "../fruit/plane.hpp"
#include "../fruit/cut_mesh.hpp"
#include "../fruit/hull/trail_intersection.hpp"
#include "../fruit/hull/projected.hpp"
#include "../fruit/hull/ring.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include "../events/render_overlay.hpp"
#include "../json/find_member.hpp"
#include "../json/parse_color.hpp"
#include "../../physics/world.hpp"
#include "../../time_format/duration_to_string.hpp"
#include "../../time_format/milliseconds.hpp"
#include "../../time_format/seconds.hpp"
#include "../../math/multiply_matrix4_vector3.hpp"
#include "../../font/color_animation.hpp"
#include "../../font/color_format.hpp"
#include "../../font/scale_animation.hpp"
#include "../../font/particle/base_parameters.hpp"
#include <sge/image/colors.hpp>
#include <sge/viewport/manager.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/float.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/time/millisecond.hpp>
#include <sge/time/unit.hpp>
#include <sge/time/second.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/unit.hpp>
#include <sge/font/pos.hpp>
#include <sge/font/text/align_h.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/font/text/align_v.hpp>
#include <sge/font/text/flags_none.hpp>
#include <sge/image/color/any/convert.hpp>
#include <sge/image/colors.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/dot.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/matrix/unproject.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/math/matrix/inverse.hpp>
#include <fcppt/math/matrix/output.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>
#include <boost/next_prior.hpp>
#include <iostream>

fruitcut::app::states::running::running(
	my_context ctx)
:
	my_base(
		ctx),
	scoped_render_state_(
		context<machine>().systems().renderer(),
		sge::renderer::state::list
			(sge::renderer::state::depth_func::less)
			(sge::renderer::state::cull_mode::off)
			(sge::renderer::state::bool_::clear_zbuffer = true)
			(sge::renderer::state::float_::zbuffer_clear_val = 1.0f)),
	line_drawer_(
		context<machine>().systems().renderer()),
	cursor_trail_(
		*context<machine>().systems().cursor_demuxer(),
		sge::time::millisecond(
			json::find_member<sge::time::unit>(
				context<machine>().config_file(),
				FCPPT_TEXT("mouse/trail-update-rate-ms"))),
		json::find_member<cursor_trail::size_type>(
				context<machine>().config_file(),
				FCPPT_TEXT("mouse/trail-samples")),
		context<machine>().systems().renderer()->onscreen_target()),
	viewport_change_connection_(
		context<machine>().systems().viewport_manager().manage_callback(
			std::tr1::bind(
				&running::viewport_change,
				this))),
	timer_font_(
		fruitcut::font::particle::base_parameters(
			context<ingame>().font_system(),
			FCPPT_TEXT("timer"),
			SGE_FONT_TEXT_LIT("you shouldn't see this!"),
			sge::font::rect::null(),
			sge::font::text::align_h::center,
			sge::font::text::align_v::center,
			sge::font::text::flags::none),
		fcppt::assign::make_container<font::color_animation::value_sequence>
			(font::color_animation::value_type(
				sge::time::second(1),
				json::parse_color<sge::image::color::rgba8>(
					json::find_member<sge::parse::json::value>(
						context<machine>().config_file(),
						FCPPT_TEXT("ingame/timer-font-color"))))),
		fcppt::assign::make_container<font::scale_animation::value_sequence>
			(font::scale_animation::value_type(
				sge::time::second(
					1),
				static_cast<sge::renderer::scalar>(
					1))),
		context<machine>().timer_callback()),
	fruit_spawned_connection_(
		context<ingame>().fruit_spawner().spawn_callback(
			std::tr1::bind(
				&audio::sound_controller::play,
				&context<machine>().sound_controller(),
				fcppt::string(FCPPT_TEXT("fruit-was-spawned"))))),
	draw_mouse_trail_(
		json::find_member<bool>(
			context<machine>().config_file(),
			FCPPT_TEXT("ingame/draw-mouse-trail"))),
	draw_bbs_(
		json::find_member<bool>(
			context<machine>().config_file(),
			FCPPT_TEXT("ingame/draw-bbs")))
{
	context<machine>().postprocessing().active(
		true);
	context<machine>().music_controller().play_random();
	viewport_change();
}

boost::statechart::result
fruitcut::app::states::running::react(
	events::render const &)
{
	context<machine>().background().render();
	context<machine>().particle_system().render();
	context<ingame>().fruit_manager().render(
		context<ingame>().camera().mvp());
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::running::react(
	events::render_overlay const &)
{
	context<ingame>().physics_debugger().render();
	line_drawer_.render_screen_space();
	context<ingame>().font_system().render();
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::running::react(
	events::toggle_pause const &)
{
	return transit<paused>();
}

boost::statechart::result
fruitcut::app::states::running::react(
	events::tick const &d)
{
	context<machine>().sound_controller().update();
	context<machine>().music_controller().update();
	context<ingame>().camera().update(
		d.delta_ms());
	context<ingame>().fruit_spawner().update();
	context<ingame>().font_system().update();
	context<machine>().particle_system().update();
	context<ingame>().physics_world().update(
		d.delta());

	timer_font_.text(
		time_format::duration_to_string<sge::font::text::string>(
			sge::time::duration(
				context<ingame>().turn_timer().time_left()),
			time_format::seconds + SGE_FONT_TEXT_LIT(":") + time_format::milliseconds));
	context<ingame>().physics_debugger().update();
	cursor_trail_.update();

	if(draw_bbs_ || draw_mouse_trail_)
	{
		line_drawer::scoped_lock slock(
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
			context<ingame>().fruit_manager().fruits().begin(); 
		i != context<ingame>().fruit_manager().fruits().end(); 
		++i)
		process_fruit(
			*i);
	context<ingame>().fruit_manager().update();

	if(context<ingame>().turn_timer().expired())
	{
		context<machine>().last_game_score(
			context<ingame>().score());
		return transit<states::gameover::superstate>();
	}
	return discard_event();
}

fruitcut::app::states::running::~running()
{
}

void
fruitcut::app::states::running::draw_fruit_bbs(
	line_drawer::line_sequence &lines)
{
	for(
		fruit::object_sequence::const_iterator i = 
			context<ingame>().fruit_manager().fruits().begin(); 
		i != context<ingame>().fruit_manager().fruits().end();
		++i) 
	{
		fruit::hull::ring const hull = 
			fruit::hull::projected(
				*i,
				context<machine>().systems().renderer()->onscreen_target(),
				context<ingame>().camera().mvp());

		for(
			fruit::hull::ring::const_iterator hull_point = hull.begin(); 
			hull_point != boost::prior(hull.end()); 
			++hull_point)
		{
			lines.push_back(
				line_drawer::line(
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
fruitcut::app::states::running::draw_mouse_trail(
	line_drawer::line_sequence &lines)
{
	if (cursor_trail_.positions().empty())
		return;

	for(
		cursor_trail::position_buffer::const_iterator i = 
			cursor_trail_.positions().begin(); 
		i != boost::prior(cursor_trail_.positions().end()); 
		++i)
		lines.push_back(
			line_drawer::line(
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
fruitcut::app::states::running::process_fruit(
	fruit::object const &current_fruit)
{
	fruit::hull::intersection_pair const intersection = 
		fruit::hull::trail_intersection(
			fruit::hull::projected(
				current_fruit,
				context<machine>().systems().renderer()->onscreen_target(),
				context<ingame>().camera().mvp()),
			cursor_trail_.positions());

	if (!intersection)
		return;

	sge::renderer::matrix4 const inverse_mvp =
		fcppt::math::matrix::inverse(
			context<ingame>().camera().mvp());

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
						context<machine>().systems().renderer()->onscreen_target()->viewport().get().size()))),
		point2_unprojected = 
			fcppt::math::matrix::unproject(
				point2,
				inverse_mvp,
				// The points are already "un-viewported", but they are in
				// screenspace, so use the screen rect here
				fcppt::math::box::basic<sge::renderer::scalar,2>(
					sge::renderer::vector2::null(),
					fcppt::math::dim::structure_cast<dim2>(
						context<machine>().systems().renderer()->onscreen_target()->viewport().get().size()))),
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
						context<machine>().systems().renderer()->onscreen_target()->viewport().get().size()))),
		first_plane_vector = 
			point2_unprojected - point1_unprojected,
		second_plane_vector = 
			point3_unprojected - point1_unprojected,
		// NOTE: For rotation matrices M and vectors a,b the following holds:
		// cross(M*a,M*b) = M*cross(a,b)
		plane_normal = 
			math::multiply_matrix4_vector3(
				fcppt::math::matrix::transpose(
					current_fruit.rotation()),
				fcppt::math::vector::cross(
					first_plane_vector,
					second_plane_vector));

	sge::renderer::scalar const plane_scalar = 
		fcppt::math::vector::dot(
			math::multiply_matrix4_vector3(
				fcppt::math::matrix::transpose(
					current_fruit.rotation()),
				point1_unprojected - current_fruit.position()),
			plane_normal);
		
	context<ingame>().fruit_manager().cut(
		current_fruit,
		fruit::plane(
			plane_normal,
			plane_scalar),
		cursor_trail_.expiry_duration(),
		context<machine>().timer_callback());

	//cursor_trail_.clear();
}

void
fruitcut::app::states::running::viewport_change()
{
	cursor_trail_.clear();

	sge::font::dim const &viewport_dim = 
		fcppt::math::dim::structure_cast<sge::font::dim>(
			context<machine>().systems().renderer()->onscreen_target()->viewport().get().size());

	timer_font_.bounding_box(
		sge::font::rect(
			sge::font::pos::null(),
			sge::font::dim(
				viewport_dim.w(),
				static_cast<sge::font::unit>(
					static_cast<sge::renderer::scalar>(
						viewport_dim.h()) * 
					static_cast<sge::renderer::scalar>(
						0.2)))));
}
