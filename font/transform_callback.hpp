#ifndef FRUITCUT_FONT_TRANSFORM_CALLBACK_HPP_INCLUDED
#define FRUITCUT_FONT_TRANSFORM_CALLBACK_HPP_INCLUDED

#include <sge/font/rect.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/function/object.hpp>
#include <boost/function.hpp>

namespace fruitcut
{
namespace font
{
typedef
boost::function
<
	// Returns: New sprite position and size
	// Gets: Font Bounding Rect (as pos/dim pair), character rect
	sge::font::rect const(
		sge::font::rect const &,
		sge::font::rect const &)
>
transform_callback;
}
}

#endif
