#ifndef FRUITAPP_POINT_SPRITE_TYPE_CHOICES_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_TYPE_CHOICES_HPP_INCLUDED

#include <sge/renderer/scalar.hpp>
#include <sge/sprite/config/float_type.hpp>
#include <sge/sprite/config/type_choices.hpp>
#include <sge/sprite/config/unit_type.hpp>


namespace fruitapp
{
namespace point_sprite
{
typedef
sge::sprite::config::type_choices
<
	sge::sprite::config::unit_type
	<
		sge::renderer::scalar
	>,
	sge::sprite::config::float_type
	<
		sge::renderer::scalar
	>
>
type_choices;
}
}

#endif
