#ifndef FRUITLIB_MATH_VIEW_PLANE_RECT_HPP_INCLUDED
#define FRUITLIB_MATH_VIEW_PLANE_RECT_HPP_INCLUDED

#include <fruitlib/perspective_projection_information.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/math/box/object_impl.hpp>

namespace fruitlib
{
namespace math
{
fcppt::math::box::object<sge::renderer::scalar,2> const
view_plane_rect(
	sge::renderer::matrix4 const &,
	fruitlib::perspective_projection_information const &);
}
}

#endif
