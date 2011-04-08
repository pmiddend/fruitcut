#ifndef FRUITCUT_FONT_DRAWER_PARAMETERS_HPP_INCLUDED
#define FRUITCUT_FONT_DRAWER_PARAMETERS_HPP_INCLUDED

#include "transform_callback.hpp"
#include <sge/image/color/any/object.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <fcppt/variant/object.hpp>

namespace fruitcut
{
namespace font
{
namespace drawer
{
class parameters
{
public:
	explicit
	parameters(
		sge::renderer::device_ptr);

	sge::renderer::device_ptr const
	renderer() const;

	void
	color(
		sge::image::color::any::object const &);

	sge::image::color::any::object const &
	color() const;

	void
	transform_callback(
		fruitcut::font::drawer::transform_callback const &);

	fruitcut::font::drawer::transform_callback const &
	transform_callback() const;
private:
	sge::renderer::device_ptr renderer_;
	sge::image::color::any::object color_;
	fruitcut::font::drawer::transform_callback transform_callback_;
};
}
}
}

#endif
