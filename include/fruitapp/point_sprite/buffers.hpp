#ifndef FRUITAPP_POINT_SPRITE_BUFFERS_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_BUFFERS_HPP_INCLUDED

#include <fruitapp/point_sprite/choices.hpp>
#include <sge/sprite/buffers/single_fwd.hpp>
#include <sge/sprite/buffers/with_declaration_fwd.hpp>


namespace fruitapp
{
namespace point_sprite
{
typedef
sge::sprite::buffers::with_declaration
<
	sge::sprite::buffers::single
	<
		fruitapp::point_sprite::choices
	>
>
buffers;
}
}

#endif
