#ifndef FRUITLIB_MATH_BOX_RADIUS_HPP_INCLUDED
#define FRUITLIB_MATH_BOX_RADIUS_HPP_INCLUDED

#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_floating_point.hpp>

namespace fruitlib
{
namespace math
{
template<typename T,fcppt::math::size_type N>
typename
boost::enable_if_c
<
	boost::is_floating_point<T>::value,
	T
>::type
box_radius(
	fcppt::math::box::basic<T,N> const &b)
{
	return 
		fcppt::math::vector::length(
			fcppt::math::dim::structure_cast<typename fcppt::math::box::basic<T,N>::vector>(
				b.size()/static_cast<T>(2)));
}
}
}

#endif
