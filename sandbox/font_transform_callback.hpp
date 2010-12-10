#ifndef FRUITCUT_SANDBOX_FONT_TRANSFORM_CALLBACK_HPP_INCLUDED
#define FRUITCUT_SANDBOX_FONT_TRANSFORM_CALLBACK_HPP_INCLUDED

#include <sge/font/pos.hpp>
#include <sge/font/dim.hpp>
#include <boost/function.hpp>
#include <utility>

namespace fruitcut
{
namespace sandbox
{
typedef
boost::function
<
	// Returns: New sprite position and size
	// Gets: Font Bounding Rect (as pos/dim pair), current character position, current character size
	std::pair
	<
		sge::font::pos,
		sge::font::dim
	> const (
		sge::font::pos const &,
		sge::font::dim const &,
		sge::font::pos const &,
		sge::font::dim const &)
>
font_transform_callback;
}
}

#endif
