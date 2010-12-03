#ifndef FRUITCUT_SPRITE_TYPE_CHOICES_HPP_INCLUDED
#define FRUITCUT_SPRITE_TYPE_CHOICES_HPP_INCLUDED

#include <sge/sprite/type_choices.hpp>
#include <sge/image/color/rgba8_format.hpp>

namespace fruitcut
{
namespace sprite
{
typedef
sge::sprite::type_choices
<
	int,
	float,
	sge::image::color::rgba8_format
>
type_choices;
}
}

#endif
