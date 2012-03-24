#include <fruitlib/scenic/adaptors/camera.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/camera/duration.hpp>
#include <sge/camera/first_person/object.hpp>
#include <sge/timer/elapsed_and_reset.hpp>
#include <sge/timer/parameters.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/duration.hpp>
#include <fcppt/config/external_end.hpp>


fruitlib::scenic::adaptors::camera::camera(
	scenic::optional_parent const &_parent,
	sge::camera::first_person::object &_camera,
	scenic::delta::callback const &_callback)
:
	node_base(
		_parent),
	clock_(
		_callback),
	timer_(
		scenic::delta::timer::parameters(
			clock_,
			boost::chrono::seconds(1))),
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
