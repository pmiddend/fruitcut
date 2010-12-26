#ifndef FRUITCUT_SANDBOX_FONT_DRAWER_HPP_INCLUDED
#define FRUITCUT_SANDBOX_FONT_DRAWER_HPP_INCLUDED

#include "font_transform_callback.hpp"
#include <sge/font/text/drawer.hpp>
#include <sge/font/text/symbol.hpp>
#include <sge/font/text/char_type.hpp>
#include <sge/font/pos.hpp>
#include <sge/font/dim.hpp>
#include <sge/font/size_type.hpp>
#include <sge/font/image_view.hpp>
#include <sge/texture/manager.hpp>
#include <sge/texture/const_part_ptr.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/sprite/system.hpp>
#include <sge/sprite/external_system_decl.hpp>
#include <sge/sprite/object_decl.hpp>
#include <sge/sprite/with_color.hpp>
#include <sge/sprite/with_dim.hpp>
#include <sge/sprite/with_texture.hpp>
#include <sge/sprite/type_choices.hpp>
#include <sge/sprite/choices.hpp>
#include <sge/image/color/any/object.hpp>
#include <sge/image/color/rgba8_format.hpp>
#include <fcppt/variant/object_impl.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <map>
#include <vector>

namespace fruitcut
{
namespace sandbox
{

class  font_drawer
:
	public sge::font::text::drawer
{
FCPPT_NONCOPYABLE(font_drawer)
public:
	explicit
	font_drawer(
		sge::renderer::device_ptr rend,
		sge::image::color::any::object const &,
		font_transform_callback const &);
	
	~font_drawer();
	
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
private:
	sge::texture::const_part_ptr const
	cached_texture(
		sge::font::text::char_type,
		sge::font::const_image_view const &);

	sge::renderer::device_ptr const rend_;

	sge::image::color::any::object col_;

	sge::texture::manager texman_;

	typedef 
	std::map
	<
		sge::font::text::char_type,
		sge::texture::const_part_ptr
	> 
	texture_map;

	texture_map textures_;

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

	sprite_system sys_;
	sprite_container sprites_;
	sge::font::pos bounding_pos_;
	sge::font::dim bounding_size_;
	font_transform_callback transform_;
};

}
}

#endif
