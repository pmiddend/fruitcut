#ifndef FRUITCUT_APP_POINT_SPRITE_SYSTEM_HPP_INCLUDED
#define FRUITCUT_APP_POINT_SPRITE_SYSTEM_HPP_INCLUDED

#include "choices.hpp"
#include <sge/sprite/system.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>

namespace fruitcut
{
namespace app
{
namespace point_sprite
{
typedef 
sge::sprite::system<choices>::type 
system;
}
}
}

#endif
