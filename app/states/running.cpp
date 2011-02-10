#include "running.hpp"
#include "paused.hpp"
#include "../hull_ring.hpp"
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
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/text.hpp>
#include <boost/foreach.hpp>
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

	/*
	BOOST_FOREACH(
		ingame::fruit_sequence::const_reference r,
		context<ingame>().fruits())
	{
		typedef
		boost::geometry::model::ring<hull_point_sequence::value_type>
		hull_ring;

		hull_ring hull;

		boost::geometry::convex_hull(
			fruit_projected_hull(
				r,
				context<machine>().systems().renderer()->onscreen_target(),
				context<ingame>().camera().mvp()),
			hull);
	}
	*/

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
		hull_ring hull = 
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
