#ifndef FRUITCUT_MATH_PLANE_NORMALIZE_HPP_INCLUDED
#define FRUITCUT_MATH_PLANE_NORMALIZE_HPP_INCLUDED

#include "basic.hpp"
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/size_type.hpp>

namespace fruitcut
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
