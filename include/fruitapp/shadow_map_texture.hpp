#ifndef FRUITAPP_SHADOW_MAP_TEXTURE_HPP_INCLUDED
#define FRUITAPP_SHADOW_MAP_TEXTURE_HPP_INCLUDED

#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <fcppt/strong_typedef.hpp>

namespace fruitapp
{
FCPPT_MAKE_STRONG_TYPEDEF(
	sge::renderer::texture::planar_shared_ptr,
	shadow_map_texture);
}

#endif

