#ifndef FRUITCUT_FONT_PARTICLE_PULSATING_HPP_INCLUDED
#define FRUITCUT_FONT_PARTICLE_PULSATING_HPP_INCLUDED

#include "base.hpp"
#include "base_parameters_fwd.hpp"
#include "../drawer_fwd.hpp"
#include "../color_animation.hpp"

namespace fruitcut
{
namespace font
{
namespace particle
{
class animated
:
	public base
{
public:
	explicit
	animated(
		base_parameters const &,
		color_animation::value_sequence const &);

	void
	update();

	void
	render(
		drawer &);

	bool
	dead() const;

	~animated();
private:
	color_animation color_animation_;
};
}
}
}

#endif
