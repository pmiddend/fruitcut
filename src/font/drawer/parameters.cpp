#include "parameters.hpp"
#include <sge/image/colors.hpp>
#include <boost/spirit/home/phoenix/core.hpp>

fruitcut::font::drawer::parameters::parameters(
	sge::renderer::device &_renderer)
:
	renderer_(
		_renderer),
	color_(
		sge::image::colors::white()),
	transform_callback_(
		boost::phoenix::arg_names::arg2)
{
}

sge::renderer::device &
fruitcut::font::drawer::parameters::renderer() const
{
	return renderer_;
}

void
fruitcut::font::drawer::parameters::color(
	sge::image::color::any::object const &_color)
{
	color_ = 
		_color;
}

sge::image::color::any::object const &
fruitcut::font::drawer::parameters::color() const
{
	return color_;
}

void
fruitcut::font::drawer::parameters::transform_callback(
	fruitcut::font::drawer::transform_callback const &_transform_callback)
{
	transform_callback_ = 
		_transform_callback;
}

fruitcut::font::drawer::transform_callback const &
fruitcut::font::drawer::parameters::transform_callback() const
{
	return 
		transform_callback_;
}