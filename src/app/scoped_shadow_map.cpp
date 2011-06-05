#include "scoped_shadow_map.hpp"
#include "shadow_map.hpp"
#include "background.hpp"

fruitcut::app::scoped_shadow_map::scoped_shadow_map(
	app::shadow_map &_shadow_map,
	app::background &_background)
:
	background_(
		_background)
{
	background_.shadow_texture(
		_shadow_map.texture());
	background_.shadow_mvp(
		_shadow_map.mvp());
}

fruitcut::app::scoped_shadow_map::~scoped_shadow_map()
{
	background_.shadow_texture(
		sge::renderer::texture::planar_ptr());
}
