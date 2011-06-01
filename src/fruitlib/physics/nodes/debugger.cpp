#include "debugger.hpp"
#include "../debugger.hpp"

fruitcut::fruitlib::physics::nodes::debugger::debugger(
	physics::debugger &_debugger)
:
	debugger_(
		_debugger)
{
}

fruitcut::fruitlib::physics::nodes::debugger::~debugger()
{
}

void
fruitcut::fruitlib::physics::nodes::debugger::update()
{
	debugger_.update();
}

void
fruitcut::fruitlib::physics::nodes::debugger::render()
{
	debugger_.render();
}
