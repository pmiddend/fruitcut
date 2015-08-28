#ifndef FRUITLIB_MATH_UNPROJECT_HPP_INCLUDED
#define FRUITLIB_MATH_UNPROJECT_HPP_INCLUDED

#include <fcppt/math/size_type.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <fcppt/math/vector/object_impl.hpp>


namespace fruitlib
{
namespace math
{
/**
	Does roughly the same as gluUnproject

	See:

	http://www.opengl.org/sdk/docs/man/xhtml/gluUnProject.xml

	Note that this function only works on static vectors and matrices right now.

	@param window_coordinates Three-dimensional window-coordinates, so you can pass a depth which specifies the distance from the viewing plane (the unprojected point is, of course, not unique)
	@param inverse_mvp The already-inverted model-view-projection matrix. This is a performance improvement over gluUnproject since you might only calculate this matrix once and then reuse it
	@param viewport The viewport, conveniently expressed as a rectangle.
 */
template<typename T,typename S1,typename S2>
fcppt::math::vector::static_<
	T,
	3
>
unproject(
	fcppt::math::vector::object<T,3,S1> const &window_coordinates,
	fcppt::math::matrix::object<T,4,4,S2> const &inverse_mvp,
	fcppt::math::box::object<T,2> const &viewport)
{
	typedef
	fcppt::math::vector::static_
	<
		T,
		4
	>
	vector4;

	// Copied from
	// http://www.opengl.org/wiki/GluProject_and_gluUnProject_code
	// and adapted to the ugly static_cast fcppt style
	vector4 result =
		inverse_mvp *
		vector4(
			static_cast<T>(
				static_cast<T>(2) * (window_coordinates.x() - viewport.pos().x()) / viewport.size().w() - static_cast<T>(1)),
			static_cast<T>(
				static_cast<T>(2) * (window_coordinates.y() - viewport.pos().y()) / viewport.size().h() - static_cast<T>(1)),
			static_cast<T>(
				static_cast<T>(2) * window_coordinates.z()),
			static_cast<T>(
				static_cast<T>(1)));

	result[3] = static_cast<T>(1)/result[3];

	return
		fcppt::math::vector::static_<T,3>(
			static_cast<T>(result[3] * result[0]),
			static_cast<T>(result[3] * result[1]),
			static_cast<T>(result[3] * result[2]));
}
}
}

#endif
