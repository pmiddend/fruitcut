#ifndef FRUITAPP_POINT_SPRITE_TYPE_CHOICES_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_TYPE_CHOICES_HPP_INCLUDED

#include <fruitapp/point_sprite/color_format.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/sprite/type_choices.hpp>

namespace fruitapp
{
namespace point_sprite
{
typedef
sge::sprite::type_choices
<
	sge::renderer::scalar,
	sge::renderer::scalar,
	color_format
>
type_choices;
}
}

#endif
