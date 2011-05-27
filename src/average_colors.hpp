#ifndef FRUITCUT_AVERAGE_COLORS_HPP_INCLUDED
#define FRUITCUT_AVERAGE_COLORS_HPP_INCLUDED

#include <sge/image/color/any/object.hpp>
#include <sge/image2d/view/const_object.hpp>

namespace fruitcut
{
sge::image::color::any::object const 
average_colors(
	sge::image2d::view::const_object const &);
}

#endif
