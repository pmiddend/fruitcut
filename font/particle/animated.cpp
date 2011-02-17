#include "animated.hpp"
#include "../drawer.hpp"
#include <sge/font/text/draw.hpp>
#include <iostream>

fruitcut::font::particle::animated::animated(
	base_parameters const &params,
	color_animation::value_sequence const &_color_animation)
:
	base(
		params),
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
fruitcut::font::particle::animated::render(
	drawer &_drawer)
{
	_drawer.color(
		color_animation_.current_value());

	base::render(
		_drawer);
}

bool
fruitcut::font::particle::animated::dead() const
{
	return color_animation_.finished();
}

fruitcut::font::particle::animated::~animated()
{
}
