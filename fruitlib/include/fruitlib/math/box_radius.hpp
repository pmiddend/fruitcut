#ifndef FRUITLIB_MATH_BOX_RADIUS_HPP_INCLUDED
#define FRUITLIB_MATH_BOX_RADIUS_HPP_INCLUDED

#include <fcppt/math/size_type.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/dim/to_vector.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/utility/enable_if.hpp>
#include <fcppt/config/external_end.hpp>


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
	fcppt::math::box::object<T,N> const &b)
{
	return
		fcppt::math::vector::length(
			fcppt::math::dim::to_vector(
				b.size()/static_cast<T>(2)));
}
}
}

#endif
