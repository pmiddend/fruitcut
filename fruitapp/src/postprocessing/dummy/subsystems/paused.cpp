#include <fruitapp/postprocessing/dummy/subsystems/paused.hpp>


fruitapp::postprocessing::dummy::subsystems::paused::paused(
	fruitlib::scenic::optional_parent const &_parent)
:
	node_base(
		_parent)
{
}

void
fruitapp::postprocessing::dummy::subsystems::paused::react(
	fruitlib::scenic::events::render const &)
{
}

fruitapp::postprocessing::dummy::subsystems::paused::~paused()
{
}
