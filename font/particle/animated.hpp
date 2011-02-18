#ifndef FRUITCUT_FONT_PARTICLE_PULSATING_HPP_INCLUDED
#define FRUITCUT_FONT_PARTICLE_PULSATING_HPP_INCLUDED

#include "base.hpp"
#include "base_parameters_fwd.hpp"
#include "../drawer_fwd.hpp"
#include "../color_animation.hpp"
#include "../scale_animation.hpp"
#include <sge/time/callback.hpp>
#include <sge/time/default_callback.hpp>

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
		color_animation::value_sequence const &,
		scale_animation::value_sequence const &,
		sge::time::callback const & = sge::time::default_callback());

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
	scale_animation scale_animation_;
};
}
}
}

#endif
