#ifndef FRUITCUT_FONT_PARTICLE_PULSATING_HPP_INCLUDED
#define FRUITCUT_FONT_PARTICLE_PULSATING_HPP_INCLUDED

#include "../drawer_fwd.hpp"
#include "../color_animation.hpp"
#include <sge/font/metrics_ptr.hpp>
#include <sge/font/text/string.hpp>

namespace fruitcut
{
namespace font
{
namespace particle
{
class animated
{
public:
	explicit
	animated(
		sge::font::metrics_ptr,
		drawer &,
		sge::font::text::string const &,
		color_animation::value_sequence const &);

	void
	update();

	void
	render();

	bool
	dead() const;

	~animated();
private:
	sge::font::metrics_ptr const metrics_;
	drawer &drawer_;
	color_animation color_animation_;
};
}
}
}

#endif
