#ifndef FRUITCUT_SPRITE_ELEMENTS_HPP_INCLUDED
#define FRUITCUT_SPRITE_ELEMENTS_HPP_INCLUDED

#include <sge/sprite/with_texture.hpp>
#include <sge/sprite/with_visibility.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace sprite
{
typedef 
boost::mpl::vector2
<
	sge::sprite::with_texture,
	sge::sprite::with_visibility
> 
elements;
}
}

#endif
