#include "running.hpp"
#include "paused.hpp"
#include "../hull_point_sequence.hpp"
#include "../fruit_projected_hull.hpp"
#include "../events/tick.hpp"
#include "../geometry_traits/vector.hpp"
#include "../../physics/world.hpp"
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/float.hpp>
#include <fcppt/math/vector/output.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/algorithms/convex_hull.hpp>
#include <boost/foreach.hpp>
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
		context<machine>().systems().renderer())
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

	line_drawer_.lines().clear();

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

	//	std::cout << "points begin\n";
		for(
			hull_ring::const_iterator hull_point = hull.begin(); 
			hull_point != boost::prior(hull.end()); 
			++hull_point)
		{
//			std::cout << (*hull_point) << "\n";
			line_drawer_.lines().push_back(
				line_drawer::line(
					sge::renderer::vector3(
						hull_point->x(),
						hull_point->y(),
						0),
					sge::renderer::vector3(
						boost::next(hull_point)->x(),
						boost::next(hull_point)->y(),
						0)));
		}
//		std::cout << "points end\n";
	}

	line_drawer_.update();

	return discard_event();
}

fruitcut::app::states::running::~running()
{
}
