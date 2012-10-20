#ifndef FRUITAPP_POINT_SPRITE_STATE_PARAMETERS_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_STATE_PARAMETERS_HPP_INCLUDED

#include <fruitapp/point_sprite/state_choices.hpp>
#include <sge/sprite/state/parameters.hpp>

namespace fruitapp
{
namespace point_sprite
{
typedef
sge::sprite::state::parameters<fruitapp::point_sprite::state_choices>
state_parameters;
}
}

#endif
