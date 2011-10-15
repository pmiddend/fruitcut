#ifndef FRUITAPP_BONSU_SPRITE_CHOICES_HPP_INCLUDED
#define FRUITAPP_BONSU_SPRITE_CHOICES_HPP_INCLUDED

#include <fruitapp/bonsu/sprite/color_format.hpp>
#include <fruitapp/bonsu/scalar.hpp>
#include <sge/sprite/choices.hpp>
#include <sge/sprite/type_choices.hpp>
#include <sge/sprite/with_color.hpp>
#include <sge/sprite/with_dim.hpp>
#include <sge/sprite/with_texture.hpp>
#include <sge/sprite/intrusive/tag.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitapp
{
namespace bonsu
{
namespace sprite
{
typedef
sge::sprite::choices
<
	sge::sprite::type_choices
	<
		bonsu::scalar,
		bonsu::scalar,
		sprite::color_format
	>,
	boost::mpl::vector4
	<
		sge::sprite::with_color,
		sge::sprite::with_dim,
		sge::sprite::with_texture,
		sge::sprite::intrusive::tag
	>
>
choices;
}
}
}

#endif
