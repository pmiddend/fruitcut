#ifndef FRUITLIB_AVERAGE_COLORS_HPP_INCLUDED
#define FRUITLIB_AVERAGE_COLORS_HPP_INCLUDED

#include <sge/image/color/any/object.hpp>
#include <sge/image2d/view/const_object.hpp>

namespace fruitlib
{
sge::image::color::any::object const 
average_colors(
	sge::image2d::view::const_object const &);
}

#endif
