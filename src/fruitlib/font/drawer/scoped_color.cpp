#include <fruitlib/font/drawer/object.hpp>
#include <fruitlib/font/drawer/scoped_color.hpp>


fruitlib::font::drawer::scoped_color::scoped_color(
	drawer::object &_object,
	font::color const &_new_color)
:
	object_(
		_object),
	old_color_(
		object_.color())
{
	object_.color(
		_new_color);
}

fruitlib::font::drawer::scoped_color::~scoped_color()
{
	object_.color(
		old_color_);
}
