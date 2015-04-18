#ifndef FRUITAPP_POINT_SPRITE_CHOICES_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_CHOICES_HPP_INCLUDED

#include <fruitapp/point_sprite/elements.hpp>
#include <fruitapp/point_sprite/type_choices.hpp>
#include <sge/sprite/config/choices.hpp>
#include <sge/sprite/config/normal_size.hpp>
#include <sge/sprite/config/pos.hpp>
#include <sge/sprite/config/pos_option.hpp>
#include <sge/sprite/config/texture_size_option.hpp>


namespace fruitapp
{
namespace point_sprite
{
typedef
sge::sprite::config::choices
<
	fruitapp::point_sprite::type_choices,
	sge::sprite::config::pos
	<
		sge::sprite::config::pos_option::center
	>,
	sge::sprite::config::normal_size
	<
		sge::sprite::config::texture_size_option::never
	>,
	fruitapp::point_sprite::elements
>
choices;
}
}

#endif
