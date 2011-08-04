#ifndef FRUITLIB_ANIMATION_INTERPOLATE_LINEAR_HPP_INCLUDED
#define FRUITLIB_ANIMATION_INTERPOLATE_LINEAR_HPP_INCLUDED

#include <fcppt/math/interpolation/linear.hpp>

namespace fruitlib
{
namespace animation
{
namespace interpolate
{
struct linear
{
template<typename Float,typename T>
T const
operator()(
	Float const v,
	T const a,
	T const b)
{
	return 
		fcppt::math::interpolation::linear(
			v,
			a,
			b);
}
};
}
}
}

#endif
