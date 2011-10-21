#ifndef FRUITLIB_MATH_VIEW_PLANE_DISTANCE_HPP_INCLUDED
#define FRUITLIB_MATH_VIEW_PLANE_DISTANCE_HPP_INCLUDED

#include <sge/camera/projection/perspective.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scalar.hpp>


namespace fruitlib
{
namespace math
{
sge::renderer::scalar
view_plane_distance(
	sge::renderer::matrix4 const &,
	sge::camera::projection::perspective const &);
}
}

#endif
