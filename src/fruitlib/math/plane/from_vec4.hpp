#ifndef FRUITLIB_MATH_PLANE_FROM_VEC4_HPP_INCLUDED
#define FRUITLIB_PLANE_FROM_VEC4_HPP_INCLUDED

#include <fruitlib/math/plane/basic.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

namespace fruitlib
{
namespace math
{
namespace plane
{
template<typename T>
basic<T> const
from_vec4(
	typename fcppt::math::vector::static_<T,4>::type const &v)
{
	return
		basic<T>(
			typename basic<T>::vector(
				v[0],
				v[1],
				v[2]),
			v[3]);
}
}
}
}

#endif
