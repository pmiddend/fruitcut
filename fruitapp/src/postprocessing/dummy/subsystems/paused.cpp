#include <fruitapp/postprocessing/dummy/system.hpp>
#include <fruitapp/postprocessing/dummy/subsystems/paused.hpp>
#include <sge/renderer/context/ffp.hpp>


fruitapp::postprocessing::dummy::subsystems::paused::paused(
	fruitapp::postprocessing::dummy::system &_parent_system,
	fruitlib::scenic::optional_parent const &_parent,
	fruitapp::postprocessing::render_callback const &_render_callback)
:
	node_base(
		_parent),
	parent_system_(
		_parent_system),
	render_callback_(
		_render_callback)
{
}

void
fruitapp::postprocessing::dummy::subsystems::paused::react(
	fruitlib::scenic::events::render const &_render_event)
{
	/*
	render_callback_(
		_render_event.context());
	*/
}

fruitapp::postprocessing::dummy::subsystems::paused::~paused()
{
}
