#ifndef FRUITLIB_FONT_DRAWER_SCOPED_COLOR_HPP_INCLUDED
#define FRUITLIB_FONT_DRAWER_SCOPED_COLOR_HPP_INCLUDED

#include <fruitlib/font/drawer/object_fwd.hpp>
#include <fruitlib/font/color.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/variant/object.hpp>

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
		drawer::object &,
		font::color const &);

	~scoped_color();
private:
	drawer::object &object_;
	font::color const old_color_;
};
}
}
}

#endif
