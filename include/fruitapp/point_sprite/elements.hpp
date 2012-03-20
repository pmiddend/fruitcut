#ifndef FRUITAPP_POINT_SPRITE_ELEMENTS_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_ELEMENTS_HPP_INCLUDED

#include <fruitapp/point_sprite/color_format.hpp>
#include <sge/sprite/config/custom_texture_point_pos.hpp>
#include <sge/sprite/config/intrusive.hpp>
#include <sge/sprite/config/no_texture_point_size.hpp>
#include <sge/sprite/config/texture_level_count.hpp>
#include <sge/sprite/config/with_color.hpp>
#include <sge/sprite/config/with_depth.hpp>
#include <sge/sprite/config/with_texture_point_size.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace point_sprite
{
typedef
boost::mpl::vector4
<
	sge::sprite::config::with_texture_point_size
	<
		sge::sprite::config::texture_level_count
		<
			1u
		>,
		sge::sprite::config::custom_texture_point_pos
		<
			false
		>,
		sge::sprite::config::no_texture_point_size
	>,
	sge::sprite::config::with_color
	<
		point_sprite::color_format
	>,
	sge::sprite::config::with_depth,
	sge::sprite::config::intrusive
>
elements;
}
}

#endif
