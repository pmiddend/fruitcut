#ifndef FRUITAPP_POINT_SPRITE_SYSTEM_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_SYSTEM_HPP_INCLUDED

#include <fruitapp/point_sprite/choices.hpp>
#include <sge/sprite/system.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>

namespace fruitapp
{
namespace point_sprite
{
typedef 
sge::sprite::system<choices>::type 
system;
}
}

#endif
