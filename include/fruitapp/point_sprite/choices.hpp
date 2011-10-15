#ifndef FRUITAPP_POINT_SPRITE_CHOICES_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_CHOICES_HPP_INCLUDED

#include <fruitapp/point_sprite/type_choices.hpp>
#include <fruitapp/point_sprite/elements.hpp>
#include <sge/sprite/choices.hpp>

namespace fruitapp
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

#endif
