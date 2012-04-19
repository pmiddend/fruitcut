#ifndef FRUITLIB_MATH_VIEW_PLANE_DISTANCE_HPP_INCLUDED
#define FRUITLIB_MATH_VIEW_PLANE_DISTANCE_HPP_INCLUDED

#include <fruitlib/perspective_projection_information.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scalar.hpp>


namespace fruitlib
{
namespace math
{
sge::renderer::scalar
view_plane_distance(
	sge::renderer::matrix4 const &,
	fruitlib::perspective_projection_information const &);
}
}

#endif
