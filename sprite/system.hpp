#ifndef FRUITCUT_SPRITE_SYSTEM_HPP_INCLUDED
#define FRUITCUT_SPRITE_SYSTEM_HPP_INCLUDED

#include "choices.hpp"
#include <sge/sprite/system.hpp>

namespace fruitcut
{
namespace sprite
{
typedef 
sge::sprite::system
<
	choices
>::type 
system;
}
}

#endif
