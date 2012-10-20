#ifndef FRUITAPP_POINT_SPRITE_COLLECTION_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_COLLECTION_HPP_INCLUDED

#include <fruitapp/point_sprite/choices.hpp>
#include <sge/sprite/intrusive/collection_fwd.hpp>


namespace fruitapp
{
namespace point_sprite
{
typedef
sge::sprite::intrusive::collection<fruitapp::point_sprite::choices>
collection;
}
}

#endif
