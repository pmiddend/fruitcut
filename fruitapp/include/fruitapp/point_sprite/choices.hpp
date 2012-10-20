#ifndef FRUITAPP_POINT_SPRITE_CHOICES_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_CHOICES_HPP_INCLUDED

#include <fruitapp/point_sprite/elements.hpp>
#include <fruitapp/point_sprite/type_choices.hpp>
#include <sge/sprite/config/choices.hpp>
#include <sge/sprite/config/normal_size.hpp>


namespace fruitapp
{
namespace point_sprite
{
typedef
sge::sprite::config::choices
<
	fruitapp::point_sprite::type_choices,
	sge::sprite::config::normal_size,
	fruitapp::point_sprite::elements
>
choices;
}
}

#endif
