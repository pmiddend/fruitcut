#ifndef FRUITLIB_PHYSICS_MATRIX4_TO_MATRIX3_HPP_INCLUDED
#define FRUITLIB_PHYSICS_MATRIX4_TO_MATRIX3_HPP_INCLUDED

#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/matrix/row.hpp>
#include <fcppt/math/matrix/static.hpp>


namespace fruitlib
{
namespace physics
{
template<typename T>
fcppt::math::matrix::static_<T,3,3>
matrix4_to_matrix3(
	fcppt::math::matrix::static_<T,4,4> const &m)
{
	return
		fcppt::math::matrix::static_<T,3,3>(
			fcppt::math::matrix::row(
				m.get_unsafe(0).get_unsafe(0),m.get_unsafe(0).get_unsafe(1),m.get_unsafe(0).get_unsafe(2)
			),
			fcppt::math::matrix::row(
				m.get_unsafe(1).get_unsafe(0),m.get_unsafe(1).get_unsafe(1),m.get_unsafe(1).get_unsafe(2)
			),
			fcppt::math::matrix::row(
				m.get_unsafe(2).get_unsafe(0),m.get_unsafe(2).get_unsafe(1),m.get_unsafe(2).get_unsafe(2)
			)
		);
}
}
}

#endif
