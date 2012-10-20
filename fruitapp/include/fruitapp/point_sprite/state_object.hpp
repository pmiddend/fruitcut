#ifndef FRUITAPP_POINT_SPRITE_STATE_OBJECT_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_STATE_OBJECT_HPP_INCLUDED

#include <fruitapp/point_sprite/state_choices.hpp>
#include <sge/sprite/state/object.hpp>

namespace fruitapp
{
namespace point_sprite
{
typedef
sge::sprite::state::object<fruitapp::point_sprite::state_choices>
state_object;
}
}

#endif
