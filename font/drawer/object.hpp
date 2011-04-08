#ifndef FRUITCUT_FONT_DRAWER_OBJECT_HPP_INCLUDED
#define FRUITCUT_FONT_DRAWER_OBJECT_HPP_INCLUDED

#include "transform_callback.hpp"
#include "parameters_fwd.hpp"
#include <sge/font/font.hpp>
#include <sge/texture/texture.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/sprite/sprite.hpp>
#include <sge/image/color/color.hpp>
#include <fcppt/variant/object_impl.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/box/box.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <map>
#include <vector>

namespace fruitcut
{
namespace font
{
namespace drawer
{
class object
:
	public sge::font::text::drawer
{
FCPPT_NONCOPYABLE(
	object);
public:
	explicit
	object(
		parameters const &);
	
	~object();
	
	void
	begin_rendering(
		sge::font::text::drawer::size_type buffer_chars,
		sge::font::pos const &start,
		sge::font::dim const &size);
	
	void
	draw_char(
		sge::font::text::char_type,
		sge::font::pos const &,
		sge::font::const_image_view const &);

	void
	end_rendering();

	void
	color(
		sge::image::color::any::object const &);

	sge::image::color::any::object const &
	color() const;

	void
	transform_callback(
		fruitcut::font::drawer::transform_callback const &);

	fruitcut::font::drawer::transform_callback const &
	transform_callback() const;
private:
	typedef 
	std::map
	<
		sge::font::text::char_type,
		sge::texture::const_part_ptr
	> 
	texture_map;

	typedef 
	sge::sprite::choices
	<
		sge::sprite::type_choices
		<
			int,
			float,
			sge::image::color::rgba8_format
		>,
		boost::mpl::vector3
		<
			sge::sprite::with_color,
			sge::sprite::with_dim,
			sge::sprite::with_texture
		>
	> 
	sprite_choices;

	typedef 
	sge::sprite::system<sprite_choices>::type 
	sprite_system;

	typedef 
	sge::sprite::object<sprite_choices> 
	sprite_object;

	typedef 
	std::vector<sprite_object> 
	sprite_container;

	sge::renderer::device_ptr const renderer_;
	sge::image::color::any::object color_;
	sge::texture::manager texture_manager_;
	texture_map textures_;
	sprite_system sprite_system_;
	sprite_container sprites_;
	sge::font::rect bounding_rect_;
	fruitcut::font::drawer::transform_callback transform_callback_;

	sge::texture::const_part_ptr const
	cached_texture(
		sge::font::text::char_type,
		sge::font::const_image_view const &);

};
}
}
}

#endif
