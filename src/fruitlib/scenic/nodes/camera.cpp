#include "camera.hpp"
#include <sge/camera/object.hpp>
#include <sge/time/second.hpp>
#include <sge/time/activation_state.hpp>

fruitcut::fruitlib::scenic::nodes::camera::camera(
	sge::camera::object &_object,
	sge::time::callback const &_time_callback)
:
	object_(
		_object),
	timer_(
		sge::time::second(1),
		sge::time::activation_state::active,
		_time_callback)
{
}

fruitcut::fruitlib::scenic::nodes::camera::~camera()
{
}

void
fruitcut::fruitlib::scenic::nodes::camera::update()
{
	object_.update(
		timer_.reset());
}

void
fruitcut::fruitlib::scenic::nodes::camera::render()
{
}
