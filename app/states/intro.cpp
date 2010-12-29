#include "intro.hpp"
#include <iostream>

fruitcut::app::states::intro::intro(
	my_context ctx)
:
	my_base(
		ctx)
{
}

boost::statechart::result
fruitcut::app::states::intro::react(
	events::render const &)
{
	context<machine>().particle_system().render();
	return discard_event();
}
