#include <fruitlib/physics/nodes/world.hpp>
#include <fruitlib/physics/world.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/elapsed_and_reset.hpp>
#include <fcppt/chrono/seconds.hpp>
#include <iostream>

fruitlib::physics::nodes::world::world(
	scenic::optional_parent const &_parent,
	scenic::delta::callback const &_time_callback,
	physics::world &_world)
:
	node_base(
		_parent),
	clock_(
		_time_callback),
	timer_(
		scenic::delta::timer::parameters(
			clock_,
			fcppt::chrono::seconds(1))),
	world_(
		_world)
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
		sge::timer::elapsed_and_reset<physics::duration>(
			timer_));
}
