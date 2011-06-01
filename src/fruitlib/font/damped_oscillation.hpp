#if 0
#ifndef FRUITCUT_FRUITLIB_FONT_DAMPED_OSCILLATION_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_FONT_DAMPED_OSCILLATION_HPP_INCLUDED

#include "scale_animation.hpp"
#include <sge/time/millisecond.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace font
{
font::scale_animation::value_sequence
const
damped_oscillation(
	//length of one period in milliseconds
	sge::time::unit const wavelength,
	//total number of frames to generate
	unsigned int const frames,
	//scaling factor, should be between 0 and 1
	float const amplitude
);
}
}
}

#endif
#endif
