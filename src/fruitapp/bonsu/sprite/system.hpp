#ifndef FRUITAPP_BONSU_SPRITE_SYSTEM_HPP_INCLUDED
#define FRUITAPP_BONSU_SPRITE_SYSTEM_HPP_INCLUDED

#include <fruitapp/bonsu/sprite/choices.hpp>
#include <sge/sprite/system.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>

namespace fruitapp
{
namespace bonsu
{
namespace sprite
{
typedef
sge::sprite::system<sprite::choices>::type
system;
}
}
}

#endif
