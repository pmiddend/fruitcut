#include "scoped_shadow_map.hpp"
#include "background.hpp"

fruitcut::app::scoped_shadow_map::scoped_shadow_map(
	sge::renderer::texture::planar &t,
	app::background &_background)
:
	background_(
		_background)
{
	background_.shadow_texture(
		&t);
}

fruitcut::app::scoped_shadow_map::~scoped_shadow_map()
{
	background_.shadow_texture(
		0);
}
