#include <fruitlib/scenic/adaptors/camera.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/camera/duration.hpp>
#include <sge/camera/object.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/elapsed_and_reset.hpp>
#include <fcppt/chrono/seconds.hpp>

fruitlib::scenic::adaptors::camera::camera(
	scenic::optional_parent const &_parent,
	sge::camera::object &_camera,
	scenic::delta::callback const &_callback)
:
	node_base(
		_parent),
	clock_(
		_callback),
	timer_(
		scenic::delta::timer::parameters(
			clock_,
			fcppt::chrono::seconds(1))),
	camera_(
		_camera)
{
}

fruitlib::scenic::adaptors::camera::~camera()
{
}

void
fruitlib::scenic::adaptors::camera::react(
	scenic::events::update const &)
{
	clock_.update();
	camera_.update(
		sge::timer::elapsed_and_reset<sge::camera::duration>(
			timer_));
}
