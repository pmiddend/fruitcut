#include "world.hpp"
#include "../world.hpp"
#include <sge/time/second.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/activation_state.hpp>

fruitcut::physics::nodes::world::world(
	fruitcut::physics::world &_world,
	sge::time::callback const &_time_callback)
:
	world_(
		_world),
	timer_(
		sge::time::second(1),
		sge::time::activation_state::active,
		_time_callback)
{
}

fruitcut::physics::nodes::world::~world()
{
}

void
fruitcut::physics::nodes::world::update()
{
	world_.update(
		sge::time::duration(
			timer_.callback()() - timer_.last_time()));
}

void
fruitcut::physics::nodes::world::render()
{
	
}

