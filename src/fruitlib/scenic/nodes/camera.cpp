#include "camera.hpp"
#include <sge/camera/object.hpp>
#include <sge/time/second.hpp>
#include <sge/time/activation_state.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <iostream>

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
	//std::cerr << "modelview: " << object_.rotation() << "\n";
	//std::cerr << "pos: " << object_.gizmo().position() << "\n";

	object_.update(
		sge::camera::duration(
			static_cast<sge::camera::duration::rep>(
				timer_.reset())));
}

void
fruitcut::fruitlib::scenic::nodes::camera::render()
{
}
