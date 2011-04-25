#include "debugger.hpp"
#include "../debugger.hpp"

fruitcut::physics::nodes::debugger::debugger(
	fruitcut::physics::debugger &_debugger)
:
	debugger_(
		_debugger)
{
}

fruitcut::physics::nodes::debugger::~debugger()
{
}

void
fruitcut::physics::nodes::debugger::update()
{
	debugger_.update();
}

void
fruitcut::physics::nodes::debugger::render()
{
	debugger_.render();
}
