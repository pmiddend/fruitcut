#include "paused.hpp"
#include "running.hpp"
#include <iostream>

fruitcut::app::states::paused::paused(
	my_context ctx)
:
	my_base(
		ctx)
{
	context<machine>().postprocessing().active(
		false);
}

boost::statechart::result
fruitcut::app::states::paused::react(
	events::render const &)
{
	context<machine>().particle_system().render();
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::paused::react(
	events::tick const &)
{
	context<machine>().particle_system().update();
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::paused::react(
	events::toggle_pause const &)
{
	return transit<running>();
}

fruitcut::app::states::paused::~paused()
{
}
