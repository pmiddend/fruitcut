#include "running.hpp"
#include <iostream>

fruitcut::app::states::running::running(
	my_context ctx)
:
	my_base(
		ctx)
{
	std::cout << "In running state\n";
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
	events::tick const &)
{
	context<machine>().particle_system().update();
	return discard_event();
}

fruitcut::app::states::running::~running()
{
}
