#include <fruitlib/physics/debugger.hpp>
#include <fruitlib/physics/nodes/debugger.hpp>
#include <fruitlib/scenic/events/render.hpp>


fruitlib::physics::nodes::debugger::debugger(
	fruitlib::scenic::optional_parent const &_parent,
	fruitlib::physics::debugger &_debugger)
:
	node_base(
		_parent),
	debugger_(
		_debugger)
{
}

fruitlib::physics::nodes::debugger::~debugger()
{
}

void
fruitlib::physics::nodes::debugger::react(
	fruitlib::scenic::events::update const &)
{
	debugger_.update();
}

void
fruitlib::physics::nodes::debugger::react(
	fruitlib::scenic::events::render const &_render_event)
{
	debugger_.render(
		_render_event.context());
}
