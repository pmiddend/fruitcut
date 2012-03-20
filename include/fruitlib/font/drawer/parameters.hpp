#ifndef FRUITLIB_FONT_DRAWER_PARAMETERS_HPP_INCLUDED
#define FRUITLIB_FONT_DRAWER_PARAMETERS_HPP_INCLUDED

#include <fruitlib/font/color.hpp>
#include <fruitlib/font/drawer/transform_callback.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <fcppt/nonassignable.hpp>
#include <fcppt/variant/object.hpp>


namespace fruitlib
{
namespace font
{
namespace drawer
{
class parameters
{
FCPPT_NONASSIGNABLE(
	parameters);
public:
	explicit
	parameters(
		sge::renderer::device &);

	sge::renderer::device &
	renderer() const;

	void
	color(
		font::color const &);

	font::color const &
	color() const;

	void
	transform_callback(
		drawer::transform_callback const &);

	drawer::transform_callback const &
	transform_callback() const;
private:
	sge::renderer::device &renderer_;
	font::color color_;
	drawer::transform_callback transform_callback_;
};
}
}
}

#endif
