#ifndef FRUITCUT_SPRITE_ELEMENTS_HPP_INCLUDED
#define FRUITCUT_SPRITE_ELEMENTS_HPP_INCLUDED

#include <sge/sprite/with_texture.hpp>
#include <sge/sprite/with_color.hpp>
#include <sge/sprite/with_rotation.hpp>
#include <sge/sprite/with_dim.hpp>
#include <sge/sprite/with_visibility.hpp>
#include <sge/sprite/intrusive/tag.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace sprite
{
typedef 
boost::mpl::vector6
<
	sge::sprite::with_texture,
	sge::sprite::with_color,
	sge::sprite::with_visibility,
	sge::sprite::with_dim,
	sge::sprite::with_rotation,
	sge::sprite::intrusive::tag
> 
elements;
}
}

#endif
