#include "line.hpp"
#include <sge/image/color/any/convert.hpp>

fruitcut::line_drawer::line::line(
	sge::renderer::vector3 const &_begin,
	sge::renderer::vector3 const &_end,
	sge::image::color::any::object const &_begin_color,
	sge::image::color::any::object const &_end_color)
:
	begin_(
		_begin),
	end_(
		_end),
	begin_color_(
		sge::image::color::any::convert<color_format>(
			_begin_color)),
	end_color_(
		sge::image::color::any::convert<color_format>(
			_end_color))
{
}

sge::renderer::vector3 const &
fruitcut::line_drawer::line::begin() const
{
	return begin_;
}

sge::renderer::vector3 const &
fruitcut::line_drawer::line::end() const
{
	return end_;
}

fruitcut::line_drawer::color const &
fruitcut::line_drawer::line::begin_color() const
{
	return begin_color_;
}

fruitcut::line_drawer::color const &
fruitcut::line_drawer::line::end_color() const
{
	return end_color_;
}
