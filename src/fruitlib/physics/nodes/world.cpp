#include "world.hpp"
#include "../world.hpp"
#include "../../scenic/events/update.hpp"
#include <sge/time/second.hpp>
#include <sge/time/second_f.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/activation_state.hpp>
#include <iostream>

fruitlib::physics::nodes::world::world(
	scenic::optional_parent const &_parent,
	physics::world &_world,
	sge::time::callback const &_time_callback)
:
	node_base(
		_parent),
	world_(
		_world),
	timer_(
		sge::time::second(1),
		sge::time::activation_state::active,
		_time_callback)
{
}

fruitlib::physics::nodes::world::~world()
{
}

void
fruitlib::physics::nodes::world::react(
	scenic::events::update const &)
{
	world_.update(
		sge::time::second_f(
			timer_.reset()));
}
