#ifndef FRUITCUT_FRUITLIB_FONT_DRAWER_TRANSFORM_CALLBACK_FN_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_FONT_DRAWER_TRANSFORM_CALLBACK_FN_HPP_INCLUDED

#include <sge/font/rect.hpp>
#include <fcppt/math/box/box.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace font
{
namespace drawer
{
typedef
// Returns: New sprite position and size
// Gets: Font Bounding Rect (as pos/dim pair), character rect
sge::font::rect const transform_callback_fn(
	sge::font::rect const &,
	sge::font::rect const &);
}
}
}
}

#endif
