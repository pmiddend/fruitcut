#ifndef FRUITCUT_SPRITE_ELEMENTS_HPP_INCLUDED
#define FRUITCUT_SPRITE_ELEMENTS_HPP_INCLUDED

#include <sge/sprite/with_texture.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace sprite
{
typedef 
boost::mpl::vector1
<
	sge::sprite::with_texture
> 
elements;
}
}

#endif
