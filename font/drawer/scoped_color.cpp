#include "scoped_color.hpp"
#include "object.hpp"

fruitcut::font::drawer::scoped_color::scoped_color(
	object &_object,
	sge::image::color::any::object const &_new_color)
:
	object_(
		_object),
	old_color_(
		object_.color())
{
	object_.color(
		_new_color);
}

fruitcut::font::drawer::scoped_color::~scoped_color()
{
	object_.color(
		old_color_);
}
