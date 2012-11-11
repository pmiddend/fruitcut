#ifndef FRUITLIB_MATH_TRANSFORM_DIRECTION_HPP_INCLUDED
#define FRUITLIB_MATH_TRANSFORM_DIRECTION_HPP_INCLUDED

#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/vector/narrow_cast.hpp>
#include <fcppt/math/vector/object_impl.hpp>

namespace fruitlib
{
namespace math
{
template<typename T,typename M,typename N,typename S1,typename S2>
fcppt::math::vector::object<T,M,S1> const
transform_direction(
	fcppt::math::matrix::object<T,N,N,S2> const &_matrix,
	fcppt::math::vector::object<T,M,S1> const &_vector)
{
	return
		fcppt::math::vector::narrow_cast
		<
			fcppt::math::vector::object<T,M,S1>
		>(
			_matrix *
			fcppt::math::vector::construct(
				_vector,
				static_cast<T>(
					0.0f)));
}
}
}

#endif
