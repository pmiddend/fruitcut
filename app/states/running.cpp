#include "running.hpp"
#include "paused.hpp"
#include <iostream>

fruitcut::app::states::running::running(
	my_context ctx)
:
	my_base(
		ctx)
{
	context<machine>().postprocessing().active(
		true);
}

boost::statechart::result
fruitcut::app::states::running::react(
	events::render const &)
{
	context<machine>().particle_system().render();
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
	events::tick const &)
{
	context<machine>().particle_system().update();
	return discard_event();
}

fruitcut::app::states::running::~running()
{
}
