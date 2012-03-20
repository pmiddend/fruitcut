#ifndef FRUITAPP_POINT_SPRITE_CHOICES_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_CHOICES_HPP_INCLUDED

#include <fruitapp/point_sprite/elements.hpp>
#include <fruitapp/point_sprite/size_attribute.hpp>
#include <fruitapp/point_sprite/type_choices.hpp>
#include <sge/sprite/config/choices.hpp>
#include <sge/sprite/config/point_size.hpp>


namespace fruitapp
{
namespace point_sprite
{
typedef
sge::sprite::config::choices
<
	type_choices,
	sge::sprite::config::point_size
	<
		point_sprite::size_attribute::dim
	>,
	elements
>
choices;
}
}

#endif
