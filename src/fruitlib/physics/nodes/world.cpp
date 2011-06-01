#include "world.hpp"
#include "../world.hpp"
#include <sge/time/second.hpp>
#include <sge/time/second_f.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/activation_state.hpp>
#include <iostream>

fruitcut::fruitlib::physics::nodes::world::world(
	physics::world &_world,
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

fruitcut::fruitlib::physics::nodes::world::~world()
{
}

void
fruitcut::fruitlib::physics::nodes::world::update()
{
	world_.update(
		sge::time::second_f(
			timer_.reset()));
}

void
fruitcut::fruitlib::physics::nodes::world::render()
{
	
}

