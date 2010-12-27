#ifndef FRUITCUT_PARTICLE_POINT_SPRITE_ELEMENTS_HPP_INCLUDED
#define FRUITCUT_PARTICLE_POINT_SPRITE_ELEMENTS_HPP_INCLUDED

#include <sge/sprite/with_texture.hpp>
#include <sge/sprite/with_color.hpp>
#include <sge/sprite/with_visibility.hpp>
#include <sge/sprite/intrusive/tag.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace particle
{
namespace point_sprite
{
typedef 
boost::mpl::vector4
<
	sge::sprite::with_texture,
	sge::sprite::with_color,
	sge::sprite::with_visibility,
	sge::sprite::intrusive::tag
> 
elements;
}
}
}

#endif
