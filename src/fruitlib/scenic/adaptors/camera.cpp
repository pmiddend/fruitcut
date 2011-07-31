#include "camera.hpp"
#include "../events/update.hpp"
#include <sge/camera/duration.hpp>
#include <sge/camera/object.hpp>
#include <fcppt/chrono/duration_cast.hpp>

fruitlib::scenic::adaptors::camera::camera(
	scenic::optional_parent const &_parent,
	sge::camera::object &_camera)
:
	node_base(
		_parent),
	camera_(
		_camera)
{
}

fruitlib::scenic::adaptors::camera::~camera()
{
}

void
fruitlib::scenic::adaptors::camera::react(
	scenic::events::update const &d)
{
	camera_.update(
		fcppt::chrono::duration_cast<sge::camera::duration>(
			d.delta()));
}
