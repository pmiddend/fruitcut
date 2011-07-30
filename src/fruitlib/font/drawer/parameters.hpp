#ifndef FRUITCUT_FRUITLIB_FONT_DRAWER_PARAMETERS_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_FONT_DRAWER_PARAMETERS_HPP_INCLUDED

#include "transform_callback.hpp"
#include "../color.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <fcppt/variant/object.hpp>
#include <fcppt/nonassignable.hpp>

namespace fruitcut
{
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
		fruitcut::fruitlib::font::drawer::transform_callback const &);

	fruitcut::fruitlib::font::drawer::transform_callback const &
	transform_callback() const;
private:
	sge::renderer::device &renderer_;
	font::color color_;
	fruitcut::fruitlib::font::drawer::transform_callback transform_callback_;
};
}
}
}
}

#endif
