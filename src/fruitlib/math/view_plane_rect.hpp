#ifndef FRUITLIB_MATH_VIEW_PLANE_RECT_HPP_INCLUDED
#define FRUITLIB_MATH_VIEW_PLANE_RECT_HPP_INCLUDED

#include <sge/camera/projection/perspective.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/math/box/box.hpp>

namespace fruitlib
{
namespace math
{
fcppt::math::box::basic<sge::renderer::scalar,2> const
view_plane_rect(
	sge::renderer::matrix4 const &,
	sge::camera::projection::perspective const &);
}
}

#endif
