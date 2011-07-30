#include "debugger.hpp"
#include "../debugger.hpp"
#include "../../scenic/events/render.hpp"
#include "../../scenic/events/update.hpp"

fruitcut::fruitlib::physics::nodes::debugger::debugger(
	fruitlib::scenic::parent const &_parent,
	physics::debugger &_debugger)
:
	node_base(
		_parent),
	debugger_(
		_debugger)
{
}

fruitcut::fruitlib::physics::nodes::debugger::~debugger()
{
}

void
fruitcut::fruitlib::physics::nodes::debugger::react(
	fruitlib::scenic::events::update const &)
{
	debugger_.update();
}

void
fruitcut::fruitlib::physics::nodes::debugger::react(
	fruitlib::scenic::events::render const &)
{
	debugger_.render();
}
