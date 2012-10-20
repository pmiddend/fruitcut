#ifndef FRUITAPP_POINT_SPRITE_ELEMENTS_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_ELEMENTS_HPP_INCLUDED

#include <fruitapp/point_sprite/color_format.hpp>
#include <sge/sprite/config/intrusive.hpp>
#include <sge/sprite/config/texture_level_count.hpp>
#include <sge/sprite/config/with_color.hpp>
#include <sge/sprite/config/with_depth.hpp>
#include <sge/sprite/config/with_texture.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace point_sprite
{
typedef
boost::mpl::vector3
<
	sge::sprite::config::with_texture
	<
		sge::sprite::config::texture_level_count
		<
			1u
			>,
		sge::sprite::config::texture_coordinates::automatic,
		sge::sprite::config::texture_ownership::reference
	>,
	sge::sprite::config::with_color
	<
		fruitapp::point_sprite::color_format
	>,
	sge::sprite::config::intrusive
>
elements;
}
}

#endif
