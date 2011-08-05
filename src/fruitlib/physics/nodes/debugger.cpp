#include <fruitlib/physics/nodes/debugger.hpp>
#include <fruitlib/physics/debugger.hpp>

fruitlib::physics::nodes::debugger::debugger(
	fruitlib::scenic::optional_parent const &_parent,
	physics::debugger &_debugger)
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
	fruitlib::scenic::events::render const &)
{
	debugger_.render();
}
