#ifndef FRUITLIB_PHYSICS_MATRIX3_TO_MATRIX4_HPP_INCLUDED
#define FRUITLIB_PHYSICS_MATRIX3_TO_MATRIX4_HPP_INCLUDED

#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/matrix/row.hpp>
#include <fcppt/math/matrix/static.hpp>


namespace fruitlib
{
namespace physics
{
template<typename T>
fcppt::math::matrix::static_<T,4,4>
matrix3_to_matrix4(
	fcppt::math::matrix::static_<T,3,3> const &m)
{
	return
		fcppt::math::matrix::static_<T,4,4>(
			fcppt::math::matrix::row(
				m.get_unsafe(0).get_unsafe(0),m.get_unsafe(0).get_unsafe(1),m.get_unsafe(0).get_unsafe(2),static_cast<T>(0)
			),
			fcppt::math::matrix::row(
				m.get_unsafe(1).get_unsafe(0),m.get_unsafe(1).get_unsafe(1),m.get_unsafe(1).get_unsafe(2),static_cast<T>(0)
			),
			fcppt::math::matrix::row(
				m.get_unsafe(2).get_unsafe(0),m.get_unsafe(2).get_unsafe(1),m.get_unsafe(2).get_unsafe(2),static_cast<T>(0)
			),
			fcppt::math::matrix::row(
				static_cast<T>(0),static_cast<T>(0),static_cast<T>(0),static_cast<T>(1)
			)
		);
}
}
}

#endif
