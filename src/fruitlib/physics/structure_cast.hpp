#ifndef FRUITLIB_PHYSICS_STRUCTURE_CAST_HPP_INCLUDED
#define FRUITLIB_PHYSICS_STRUCTURE_CAST_HPP_INCLUDED

#include <fruitlib/physics/scalar.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/math/dim/has_dim.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/matrix/has_dim.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/has_dim.hpp>
#include <fcppt/config/external_begin.hpp>
#include <LinearMath/btMatrix3x3.h>
#include <LinearMath/btVector3.h>
#include <boost/utility/enable_if.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace physics
{
template<typename Container>
typename
boost::enable_if_c
<
	fcppt::math::vector::has_dim<Container,static_cast<fcppt::math::size_type>(3)>::value ||
	fcppt::math::dim::has_dim<Container,static_cast<fcppt::math::size_type>(3)>::value,
	Container
>::type
structure_cast(
	btVector3 const &s)
{
	return
		Container(
			static_cast<typename Container::value_type>(
				s[0]),
			static_cast<typename Container::value_type>(
				s[1]),
			static_cast<typename Container::value_type>(
				s[2]));
}

template<typename Container>
typename
boost::enable_if_c
<
	fcppt::math::vector::has_dim<Container,static_cast<fcppt::math::size_type>(3)>::value ||
	fcppt::math::dim::has_dim<Container,static_cast<fcppt::math::size_type>(3)>::value,
	btVector3
>::type
structure_cast(
	Container const &c)
{
	return
		btVector3(
			static_cast<scalar>(c[0]),
			static_cast<scalar>(c[1]),
			static_cast<scalar>(c[2]));
}





template<typename Container>
typename
boost::enable_if_c
<
	fcppt::math::matrix::has_dim<Container,static_cast<fcppt::math::size_type>(3),static_cast<fcppt::math::size_type>(3)>::value,
	Container
>::type
structure_cast(
	btMatrix3x3 const &m)
{
	return
		Container(
			m[0].getX(),m[0].getY(),m[0].getZ(),
			m[1].getX(),m[1].getY(),m[1].getZ(),
			m[2].getX(),m[2].getY(),m[2].getZ());
}

template<typename Container>
typename
boost::enable_if_c
<
	fcppt::math::matrix::has_dim<Container,static_cast<fcppt::math::size_type>(3),static_cast<fcppt::math::size_type>(3)>::value,
	btMatrix3x3
>::type
structure_cast(
	Container const &m)
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
