#ifndef FRUITLIB_MATH_PLANE_NORMALIZE_HPP_INCLUDED
#define FRUITLIB_MATH_PLANE_NORMALIZE_HPP_INCLUDED

#include <fruitlib/math/plane/basic.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/normalize.hpp>


namespace fruitlib
{
namespace math
{
namespace plane
{
template<typename T,fcppt::math::size_type N>
basic<T,N> const
normalize(
	basic<T,N> const &p)
{
	return
		basic<T,N>(
			fcppt::math::vector::normalize(
				p.normal()),
			p.lambda()/
			fcppt::math::vector::length(
				p.normal()));
}
}
}
}

#endif
