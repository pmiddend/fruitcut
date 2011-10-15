#ifndef FRUITAPP_POINT_SPRITE_ELEMENTS_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_ELEMENTS_HPP_INCLUDED

#include <sge/sprite/with_texture.hpp>
#include <sge/sprite/with_color.hpp>
#include <sge/sprite/with_unspecified_dim.hpp>
#include <sge/sprite/with_depth.hpp>
#include <sge/sprite/intrusive/tag.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitapp
{
namespace point_sprite
{
typedef
boost::mpl::vector5
<
	sge::sprite::with_texture,
	sge::sprite::with_unspecified_dim,
	sge::sprite::with_color,
	sge::sprite::with_depth,
	sge::sprite::intrusive::tag
>
elements;
}
}

#endif
