#ifndef FRUITCUT_MATH_VIEW_PLANE_DISTANCE_HPP_INCLUDED
#define FRUITCUT_MATH_VIEW_PLANE_DISTANCE_HPP_INCLUDED

#include <sge/renderer/scalar.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/camera/projection/perspective.hpp>

namespace fruitcut
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
