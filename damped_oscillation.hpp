#ifndef FRUITCUT_DAMPED_OSCILLATION_HPP_INCLUDED
#define FRUITCUT_DAMPED_OSCILLATION_HPP_INCLUDED

#include "font/scale_animation.hpp"
#include <sge/time/millisecond.hpp>

namespace fruitcut
{
fruitcut::font::scale_animation::value_sequence
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

#endif
