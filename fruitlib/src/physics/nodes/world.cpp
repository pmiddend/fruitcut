#include <fruitlib/physics/world.hpp>
#include <fruitlib/physics/nodes/world.hpp>
#include <sge/timer/elapsed.hpp>
#include <sge/timer/elapsed_and_reset.hpp>
#include <sge/timer/parameters.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/duration.hpp>
#include <fcppt/config/external_end.hpp>


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
			boost::chrono::seconds(1))),
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
	clock_.update();
	world_.update(
		sge::timer::elapsed_and_reset<physics::duration>(
			timer_));
}
