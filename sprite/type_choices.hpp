#ifndef FRUITCUT_SPRITE_TYPE_CHOICES_HPP_INCLUDED
#define FRUITCUT_SPRITE_TYPE_CHOICES_HPP_INCLUDED

#include <sge/sprite/type_choices.hpp>
#include <sge/sprite/no_color.hpp>

namespace fruitcut
{
namespace sprite
{
typedef
sge::sprite::type_choices
<
	int,
	float,
	sge::sprite::no_color
>
type_choices;
}
}

#endif
