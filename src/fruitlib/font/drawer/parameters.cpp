#include "parameters.hpp"
#include <fruitlib/font/color_format.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/any/object.hpp>
#include <sge/image/color/any/convert.hpp>
#include <boost/spirit/home/phoenix/core.hpp>

fruitlib::font::drawer::parameters::parameters(
	sge::renderer::device &_renderer)
:
	renderer_(
		_renderer),
	color_(
		sge::image::color::any::convert<font::color_format>(
			sge::image::colors::white())),
	transform_callback_(
		boost::phoenix::arg_names::arg2)
{
}

sge::renderer::device &
fruitlib::font::drawer::parameters::renderer() const
{
	return renderer_;
}

void
fruitlib::font::drawer::parameters::color(
	font::color const &_color)
{
	color_ = 
		_color;
}

fruitlib::font::color const &
fruitlib::font::drawer::parameters::color() const
{
	return color_;
}

void
fruitlib::font::drawer::parameters::transform_callback(
	fruitlib::font::drawer::transform_callback const &_transform_callback)
{
	transform_callback_ = 
		_transform_callback;
}

fruitlib::font::drawer::transform_callback const &
fruitlib::font::drawer::parameters::transform_callback() const
{
	return 
		transform_callback_;
}
