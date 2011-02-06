#ifndef FRUITCUT_LINE_DRAWER_LINE_HPP_INCLUDED
#define FRUITCUT_LINE_DRAWER_LINE_HPP_INCLUDED

#include "color.hpp"
#include "color_format.hpp"
#include <sge/renderer/vector3.hpp>
#include <sge/image/color/any/object.hpp>
#include <sge/image/colors.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/variant/object.hpp>

namespace fruitcut
{
namespace line_drawer
{
class line
{
public:
	explicit
	line(
		sge::renderer::vector3 const &,
		sge::renderer::vector3 const &,
		sge::image::color::any::object const & = sge::image::colors::white(),
		sge::image::color::any::object const & = sge::image::colors::white());

	sge::renderer::vector3 const &
	begin() const;

	sge::renderer::vector3 const &
	end() const;

	color const &
	begin_color() const;

	color const &
	end_color() const;
private:
	sge::renderer::vector3 begin_,end_;
	color begin_color_,end_color_;
};
}
}

#endif
