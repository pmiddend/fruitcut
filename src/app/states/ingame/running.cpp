#include "running.hpp"
#include "paused.hpp"
#include "../gameover/superstate.hpp"
#include "../gameover/choose_name.hpp"
#include "../../dim2.hpp"
#include "../../../create_rng.hpp"
#include "../../../math/multiply_matrix4_vector3.hpp"
#include "../../../uniform_random.hpp"
#include "../../fruit/plane.hpp"
#include "../../fruit/triangle_traits.hpp"
#include "../../fruit/cut_mesh.hpp"
#include "../../fruit/hull/trail_intersection.hpp"
#include "../../fruit/hull/projected.hpp"
#include "../../fruit/hull/ring.hpp"
#include "../../events/tick.hpp"
#include "../../events/render.hpp"
#include "../../events/render_overlay.hpp"
#include "../../../json/find_member.hpp"
#include "../../../physics/world.hpp"
#include "../../../math/multiply_matrix4_vector3.hpp"
#include "../../../math/triangle/random_point.hpp"
#include "../../point_sprite/splatter/gravity_callback.hpp"
#include "../../point_sprite/splatter/parameters.hpp"
#include "../../point_sprite/splatter/object.hpp"
#include "../../point_sprite/vector.hpp"
#include "../../point_sprite/color_format.hpp"
#include <sge/line_drawer/scoped_lock.hpp>
#include <sge/line_drawer/render_to_screen.hpp>
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
#include <sge/time/second.hpp>
#include <sge/time/unit.hpp>
#include <sge/image/color/any/convert.hpp>
#include <sge/image/colors.hpp>
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
#include <fcppt/random/make_last_exclusive_range.hpp>
#include <fcppt/random/make_inclusive_range.hpp>
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
			json::find_member<sge::time::unit>(
				context<machine>().config_file(),
				FCPPT_TEXT("mouse/trail-update-rate-ms"))),
		json::find_member<cursor_trail::size_type>(
				context<machine>().config_file(),
				FCPPT_TEXT("mouse/trail-samples")),
		context<machine>().systems().renderer().onscreen_target()),
	cursor_trail_node_(
		cursor_trail_),
	update_node_(
		std::tr1::bind(
			&running::update,
			this),
		scenic::nodes::intrusive_with_callbacks::render_callback()),
	viewport_change_connection_(
		context<machine>().systems().viewport_manager().manage_callback(
			std::tr1::bind(
				&running::viewport_change,
				this))),
	fruit_spawned_connection_(
		context<superstate>().fruit_spawner().spawn_callback(
			std::tr1::bind(
				&audio::sound_controller::play,
				&context<machine>().sound_controller(),
				fcppt::string(FCPPT_TEXT("fruit-was-spawned"))))),
	fruit_cut_connection_(
		context<superstate>().fruit_manager().cut_callback(
			std::tr1::bind(
				&running::fruit_was_cut,
				this,
				std::tr1::placeholders::_1))),
	draw_mouse_trail_(
		json::find_member<bool>(
			context<machine>().config_file(),
			FCPPT_TEXT("ingame/draw-mouse-trail"))),
	draw_bbs_(
		json::find_member<bool>(
			context<machine>().config_file(),
			FCPPT_TEXT("ingame/draw-bbs")))
{
	context<machine>().overlay_node().children().push_back(
		update_node_);
	context<machine>().overlay_node().children().push_back(
		line_drawer_node_);
	context<machine>().overlay_node().children().push_back(
		cursor_trail_node_);
	context<machine>().postprocessing().active(
		true);
	context<machine>().music_controller().play_random();
	viewport_change();
}

boost::statechart::result
fruitcut::app::states::ingame::running::react(
	events::toggle_pause const &)
{
	return transit<paused>();
}

boost::statechart::result
fruitcut::app::states::ingame::running::react(
	events::tick const &)
{
	if(context<superstate>().game_logic().finished())
	{
		context<machine>().last_game_score(
			context<superstate>().game_logic().score());
		return transit<states::gameover::superstate>();
	}
	return discard_event();
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
		cursor_trail::position_buffer::const_iterator i = 
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

void
fruitcut::app::states::ingame::running::fruit_was_cut(
	fruit::cut_context const &c)
{
	if(c.cross_section().triangles.empty())
		return;

	typedef
	fruitcut::uniform_random<fruit::mesh::triangle_sequence::size_type>::type
	triangle_randomizer;

	triangle_randomizer tri_rng(
		fcppt::random::make_last_exclusive_range(
			static_cast<fruit::mesh::triangle_sequence::size_type>(
				0),
			c.cross_section().triangles.size()),
		fruitcut::create_rng());

	typedef
	fruitcut::uniform_random<sge::renderer::scalar>::type
	triangle_point_randomizer;

	triangle_point_randomizer tri_point_rng(
		fcppt::random::make_inclusive_range(
			static_cast<sge::renderer::scalar>(
				0),
			static_cast<sge::renderer::scalar>(
				1)));

	typedef
	fruitcut::uniform_random<unsigned>::type
	cut_direction_randomizer;

	cut_direction_randomizer cut_direction_rng(
		fcppt::random::make_inclusive_range(
			0u,
			1u));

	unsigned const number_of_points = 10;
	sge::renderer::scalar const speed = 200;
	point_sprite::splatter::size::value_type const size = 30;

	for(unsigned i = 0; i < number_of_points; ++i)
	{
		FCPPT_ASSERT(
			!c.cross_section().triangles.empty());
		sge::renderer::vector3 const position = 
			math::triangle::random_point(
				c.cross_section().triangles[
					tri_rng()],
				tri_point_rng);

		// FUCK, IS THAT UGLY!
		point_sprite::splatter::gravity_callback grav_callback(
			std::tr1::bind(
				&fcppt::math::vector::structure_cast
				<
					point_sprite::vector,
					physics::scalar,
					physics::vector3::dim_wrapper,
					physics::vector3::storage_type
				>,
				std::tr1::bind(
					static_cast<physics::vector3 const(physics::world::*)() const>(
						&physics::world::gravity),
					&context<superstate>().physics_world())));

		context<machine>().point_sprites().push_back(
			point_sprite::unique_base_ptr(
				fcppt::make_unique_ptr<point_sprite::splatter::object>(
					point_sprite::splatter::parameters(
						context<machine>().point_sprites().system(),
						point_sprite::splatter::position(
							c.old().position() + 
							math::multiply_matrix4_vector3(
								c.old().world_transform(),
								position)),
						point_sprite::splatter::linear_velocity(
							(cut_direction_rng() 
							? 
								c.cut_direction() 
							: 
								(-c.cut_direction())) * speed),
						point_sprite::splatter::size(
							size),
						sge::image::color::any::convert<point_sprite::color_format>(
							sge::image::colors::green()),
						context<machine>().point_sprites().lookup_texture(
							FCPPT_TEXT("splat0")),
						sge::time::second(2),
						context<machine>().timer_callback(),
						grav_callback))));
	}
}
