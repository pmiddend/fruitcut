#ifndef FRUITAPP_POINT_SPRITE_CONNECTION_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_CONNECTION_HPP_INCLUDED

#include <fruitapp/point_sprite/choices.hpp>
#include <sge/sprite/intrusive/connection_fwd.hpp>


namespace fruitapp
{
namespace point_sprite
{
typedef
sge::sprite::intrusive::connection<choices>
connection;
}
}

#endif
