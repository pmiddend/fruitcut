#ifndef FRUITLIB_MATH_MULTIPLY_MATRIX4_VECTOR3_HPP_INCLUDED
#define FRUITLIB_MATH_MULTIPLY_MATRIX4_VECTOR3_HPP_INCLUDED

#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/static.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/vector/narrow_cast.hpp>
#include <fcppt/math/vector/static.hpp>


namespace fruitlib
{
namespace math
{
// NOTE: Could this find its way to fcppt?
template<typename T,typename M1,typename M2,typename N,typename S1,typename S2>
typename
fcppt::math::vector::static_<T,3>::type const
multiply_matrix4_vector3(
	fcppt::math::matrix::basic<T,M1,M2,S1> const &m,
	fcppt::math::vector::basic<T,N,S2> const &v)
{
	return
		fcppt::math::vector::narrow_cast
		<
			typename fcppt::math::vector::static_<T,3>::type
		>(
			m *
			fcppt::math::vector::construct(
				v,
				static_cast<T>(0)));
}
}
}

#endif
