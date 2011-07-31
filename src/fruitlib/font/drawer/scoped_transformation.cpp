#include <fruitlib/font/drawer/scoped_transformation.hpp>
#include <fruitlib/font/drawer/object.hpp>

fruitlib::font::drawer::scoped_transformation::scoped_transformation(
	object &_drawer,
	transform_callback const &_new_callback)
:
	drawer_(
		_drawer),
	old_callback_(
		drawer_.transform_callback())
{
	drawer_.transform_callback(
		_new_callback);
}

fruitlib::font::drawer::scoped_transformation::~scoped_transformation()
{
	drawer_.transform_callback(
		old_callback_);
}
