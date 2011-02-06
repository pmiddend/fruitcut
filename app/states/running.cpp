#include "running.hpp"
#include "paused.hpp"
#include "../events/tick.hpp"
#include "../../physics/world.hpp"
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/float.hpp>
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
			(sge::renderer::state::float_::zbuffer_clear_val = 1.0f))
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
	return discard_event();
}

fruitcut::app::states::running::~running()
{
}
