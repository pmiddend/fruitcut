#ifndef FRUITLIB_ANIMATION_INTERPOLATE_TRIGONOMETRIC_HPP_INCLUDED
#define FRUITLIB_ANIMATION_INTERPOLATE_TRIGONOMETRIC_HPP_INCLUDED

#include <fcppt/math/interpolation/trigonometric.hpp>

namespace fruitlib
{
namespace animation
{
namespace interpolate
{
struct trigonometric
{
template<typename Float,typename T>
T const
operator()(
	Float const v,
	T const a,
	T const b)
{
	return 
		fcppt::math::interpolation::trigonometric(
			v,
			a,
			b);
}
};
}
}
}

#endif
