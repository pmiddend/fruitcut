#include "damped_oscillation.hpp"

fruitcut::font::scale_animation::value_sequence
const
fruitcut::font::damped_oscillation(
	sge::time::unit const wavelength,
	unsigned int const frames,
	float const amplitude
)
{
	fruitcut::font::scale_animation::value_sequence sequence;
	for (unsigned int t = 0; t < frames; ++t)
	{
		sequence.push_back(
			fruitcut::font::scale_animation::value_type(
				sge::time::millisecond(
					wavelength
				),
				static_cast<sge::renderer::scalar>(
					1.0f + 
					amplitude *
					((t % 2 == 0)? -1.f : 1.f) *
					std::exp(
						- static_cast<float>(t) *
						//exp(-10) small enough?
						static_cast<float>(10) /
						static_cast<float>(frames)
						))));
	}
	return sequence;
}
