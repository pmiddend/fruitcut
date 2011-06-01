#ifndef FRUITCUT_FRUITLIB_PARTICLE_POINT_SPRITE_CHOICES_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PARTICLE_POINT_SPRITE_CHOICES_HPP_INCLUDED

#include "type_choices.hpp"
#include "elements.hpp"
#include <sge/sprite/choices.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace particle
{
namespace point_sprite
{
typedef 
sge::sprite::choices
<
	type_choices,
	elements
> 
choices;
}
}
}
}

#endif
