#ifndef FRUITLIB_PHYSICS_STRUCTURE_CAST_HPP_INCLUDED
#define FRUITLIB_PHYSICS_STRUCTURE_CAST_HPP_INCLUDED

#include <fruitlib/physics/scalar.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/math/matrix/has_dim.hpp>
#include <fcppt/math/matrix/row.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/vector/has_dim.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <LinearMath/btMatrix3x3.h>
#include <LinearMath/btVector3.h>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace physics
{
template<typename Container>
Container
structure_cast(
	btVector3 const &s)
{
	static_assert(
		fcppt::math::vector::has_dim<
			Container,
			3u
		>::value,
		""
	);

	return
		Container(
			static_cast<typename Container::value_type>(
				s[0]),
			static_cast<typename Container::value_type>(
				s[1]),
			static_cast<typename Container::value_type>(
				s[2]));
}

template<
	typename T,
	typename S
>
btVector3
structure_cast(
	fcppt::math::vector::object<
		T,
		3u,
		S
	> const &c)
{
	return
		btVector3(
			static_cast<scalar>(c[0]),
			static_cast<scalar>(c[1]),
			static_cast<scalar>(c[2]));
}

template<typename Container>
Container
structure_cast(
	btMatrix3x3 const &m)
{
	static_assert(
		fcppt::math::matrix::has_dim<
			Container,
			3u,
			3u
		>::value,
		""
	);

	return
		Container(
			fcppt::math::matrix::row(
				m[0].getX(),m[0].getY(),m[0].getZ()
			),
			fcppt::math::matrix::row(
				m[1].getX(),m[1].getY(),m[1].getZ()
			),
			fcppt::math::matrix::row(
				m[2].getX(),m[2].getY(),m[2].getZ()
			)
		);
}

template<
	typename T,
	typename S
>
btMatrix3x3
structure_cast(
	fcppt::math::matrix::object<
		T,
		3,
		3,
		S
	> const &m)
{
	return
		btMatrix3x3(
			m[0][0],m[0][1],m[0][2],
			m[1][0],m[1][1],m[1][2],
			m[2][0],m[2][1],m[2][2]);
}

}
}

#endif
