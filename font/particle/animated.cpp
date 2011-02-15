#include "animated.hpp"
#include <sge/font/text/draw.hpp>

fruitcut::font::particle::animated::animated(
	sge::font::metrics_ptr const _metrics,
	drawer &_drawer,
	sge::font::text::string const &_text,
	color_animation::value_sequence const &_color_animation)
:
	metrics_(
		_metrics),
	drawer_(
		_drawer),
	text_(
		_text),
	color_animation_(
		_color_animation)
{
}

void
fruitcut::font::particle::animated::update()
{
	color_animation_.update();
}

void
fruitcut::font::particle::animated::render()
{
	sge::font::text::draw(
		metrics_,
		drawer_,
		text_,
		sge::font::pos::null(),
		fcppt::math::dim::structure_cast<sge::font::dim>(
			sys.renderer()->screen_size()),
		sge::font::text::align_h::center,
		sge::font::text::align_v::center,
		sge::font::text::flags::none);
}

bool
fruitcut::font::particle::animated::dead() const
{
	return color_animation_.finished();
}

fruitcut::font::particle::animated::~animated()
{
}
