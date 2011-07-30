#include "scoped_color.hpp"
#include "object.hpp"

fruitcut::fruitlib::font::drawer::scoped_color::scoped_color(
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

fruitcut::fruitlib::font::drawer::scoped_color::~scoped_color()
{
	object_.color(
		old_color_);
}
