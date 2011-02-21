#include "spawner.hpp"

fruitcut::app::fruit::spawner::spawner(
	sge::camera::object const &_camera,
	sge::time::callback const &_callback,
	spawn_callback const &_spawn_callback)
:
	camera_(
		_camera),
	callback_(
		_callback),
	spawn_callback_(
		_spawn_callback)
{
}

void
fruitcut::app::fruit::spawner::update()
{
}
