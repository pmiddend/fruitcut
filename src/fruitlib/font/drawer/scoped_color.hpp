#ifndef FRUITCUT_FRUITLIB_FONT_DRAWER_SCOPED_COLOR_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_FONT_DRAWER_SCOPED_COLOR_HPP_INCLUDED

#include "object_fwd.hpp"
#include <sge/image/color/any/object.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/variant/object.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace font
{
namespace drawer
{
class scoped_color
{
FCPPT_NONCOPYABLE(
	scoped_color);
public:
	explicit
	scoped_color(
		object &,
		sge::image::color::any::object const &);

	~scoped_color();
private:
	object &object_;
	sge::image::color::any::object const old_color_;
};
}
}
}
}

#endif
