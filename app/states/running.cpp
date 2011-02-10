#include "running.hpp"
#include "paused.hpp"
#include "../hull_ring.hpp"
#include "../cut_mesh.hpp"
#include "../dim2.hpp"
#include "../plane.hpp"
#include "../hull_trail_intersection.hpp"
#include "../fruit_projected_hull.hpp"
#include "../events/tick.hpp"
#include "../json/find_member.hpp"
#include "../../physics/world.hpp"
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/float.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/input/cursor/position_unit.hpp>
#include <sge/image/colors.hpp>
#include <sge/time/unit.hpp>
#include <sge/time/millisecond.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/dot.hpp>
#include <fcppt/math/vector/narrow_cast.hpp>
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/matrix/unproject.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/math/matrix/inverse.hpp>
#include <fcppt/math/matrix/output.hpp>
#include <fcppt/text.hpp>
#include <boost/foreach.hpp>
#include <boost/next_prior.hpp>
#include <iostream>

namespace
{
// NOTE: Could this find its way to fcppt?
template<typename T>
typename
fcppt::math::vector::static_<T,3>::type const
multiply_matrix4_vector3(
	typename
	fcppt::math::matrix::static_<T,4,4>::type const &m,
	typename
	fcppt::math::vector::static_<T,3>::type const &v)
{
	return 
		fcppt::math::vector::narrow_cast
		<
			typename fcppt::math::vector::static_<T,3>::type
		>(
			m * 
			fcppt::math::vector::construct(
				v,
				static_cast<T>(0)));
}
}

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
		context<machine>().systems().renderer()->onscreen_target())
{
	context<machine>().postprocessing().active(
		true);
}

boost::statechart::result
fruitcut::app::states::running::react(
	events::render const &)
{
	context<machine>().particle_system().render();
	context<ingame>().render_fruits();
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::running::react(
	events::render_overlay const &)
{
	line_drawer_.render_screen_space();
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
	context<ingame>().camera().update(
		d.delta_ms());
	context<machine>().particle_system().update();
	context<ingame>().physics_world().update(
		d.delta());
	cursor_trail_.update();

	line_drawer_.lines().clear();
	draw_fruit_bbs();
	draw_mouse_trail();
	line_drawer_.update();

	BOOST_FOREACH(
		ingame::fruit_sequence::const_reference r,
		context<ingame>().fruits())
		process_fruit(
			r);

	return discard_event();
}

fruitcut::app::states::running::~running()
{
}

void
fruitcut::app::states::running::draw_fruit_bbs()
{
	BOOST_FOREACH(
		ingame::fruit_sequence::const_reference r,
		context<ingame>().fruits())
	{
		hull_ring const hull = 
			fruit_projected_hull(
				r,
				context<machine>().systems().renderer()->onscreen_target(),
				context<ingame>().camera().mvp());

		for(
			hull_ring::const_iterator hull_point = hull.begin(); 
			hull_point != boost::prior(hull.end()); 
			++hull_point)
		{
			line_drawer_.lines().push_back(
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
							0))));
		}
	}
}

void
fruitcut::app::states::running::draw_mouse_trail()
{
	if (cursor_trail_.positions().empty())
		return;

	for(
		cursor_trail::position_buffer::const_iterator i = 
			cursor_trail_.positions().begin(); 
		i != boost::prior(cursor_trail_.positions().end()); 
		++i)
		line_drawer_.lines().push_back(
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
	fruit const &current_fruit)
{
	fcppt::optional<fcppt::homogenous_pair<fruitcut::app::hull_ring::value_type> > const intersection = 
		hull_trail_intersection(
			fruit_projected_hull(
				current_fruit,
				context<machine>().systems().renderer()->onscreen_target(),
				context<ingame>().camera().mvp()),
			cursor_trail_.positions());

	if (!intersection)
		return;

	static int counter = 0;
	std::cout << ++counter << " doing something\n";

	//std::cout <<  ++counter <<  "intersections at " << intersection->first << " and " << intersection->second << "\n";
	
	// Just for debugging purposes
	dim2 const ss = 
		fcppt::math::dim::structure_cast<dim2>(
			context<machine>().systems().renderer()->screen_size());

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
						context<machine>().systems().renderer()->screen_size()))),
		point2_unprojected = 
			fcppt::math::matrix::unproject(
				point2,
				inverse_mvp,
				// The points are already "un-viewported", but they are in
				// screenspace, so use the screen rect here
				fcppt::math::box::basic<sge::renderer::scalar,2>(
					sge::renderer::vector2::null(),
					fcppt::math::dim::structure_cast<dim2>(
						context<machine>().systems().renderer()->screen_size()))),
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
						context<machine>().systems().renderer()->screen_size()))),
		first_plane_vector = 
			point2_unprojected - point1_unprojected,
		second_plane_vector = 
			point3_unprojected - point1_unprojected,
		// NOTE: For rotation matrices M and vectors a,b the following holds:
		// cross(M*a,M*b) = M*cross(a,b)
		plane_normal = 
			multiply_matrix4_vector3<sge::renderer::scalar>(
				fcppt::math::matrix::transpose(
					current_fruit.rotation()),
				fcppt::math::vector::cross(
					first_plane_vector,
					second_plane_vector));

	sge::renderer::scalar const plane_scalar = 
		fcppt::math::vector::dot(
			multiply_matrix4_vector3<sge::renderer::scalar>(
				fcppt::math::matrix::transpose(
					current_fruit.rotation()),
				point1_unprojected - current_fruit.position()),
			plane_normal);
		
	context<ingame>().cut_fruit(
		current_fruit,
		plane(
			plane_normal,
			plane_scalar));

	cursor_trail_.clear();
}
