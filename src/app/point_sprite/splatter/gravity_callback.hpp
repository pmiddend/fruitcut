#ifndef FRUITCUT_APP_POINT_SPRITE_SPLATTER_GRAVITY_CALLBACK_HPP_INCLUDED
#define FRUITCUT_APP_POINT_SPRITE_SPLATTER_GRAVITY_CALLBACK_HPP_INCLUDED

#include "../vector.hpp"
#include <fcppt/function/object.hpp>

namespace fruitcut
{
namespace app
{
namespace point_sprite
{
namespace splatter
{
typedef
fcppt::function::object<point_sprite::vector()>
gravity_callback;
}
}
}
}

#endif
