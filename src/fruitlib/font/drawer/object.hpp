#ifndef FRUITLIB_FONT_DRAWER_OBJECT_HPP_INCLUDED
#define FRUITLIB_FONT_DRAWER_OBJECT_HPP_INCLUDED

#include <fruitlib/font/color.hpp>
#include <fruitlib/font/color_format.hpp>
#include <fruitlib/font/drawer/parameters_fwd.hpp>
#include <fruitlib/font/drawer/transform_callback.hpp>
#include <sge/font/text/drawer.hpp>
#include <sge/image/color/rgba8_format.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/sprite/object_decl.hpp>
#include <sge/sprite/buffers/single_decl.hpp>
#include <sge/sprite/buffers/with_declaration_decl.hpp>
#include <sge/sprite/config/choices.hpp>
#include <sge/sprite/config/float_type.hpp>
#include <sge/sprite/config/normal_size.hpp>
#include <sge/sprite/config/texture_coordinates.hpp>
#include <sge/sprite/config/texture_level_count.hpp>
#include <sge/sprite/config/type_choices.hpp>
#include <sge/sprite/config/unit_type.hpp>
#include <sge/sprite/config/with_color.hpp>
#include <sge/sprite/config/with_texture.hpp>
#include <sge/texture/const_part_ptr.hpp>
#include <sge/texture/manager.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/variant/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <map>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
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
		font::color const &);

	font::color const &
	color() const;

	void
	transform_callback(
		font::drawer::transform_callback const &);

	font::drawer::transform_callback const &
	transform_callback() const;
private:
	typedef
	std::map
	<
		sge::font::text::char_type,
		sge::texture::const_part_ptr
	>
	texture_map;

	typedef sge::sprite::config::choices
	<
		sge::sprite::config::type_choices
		<
			sge::sprite::config::unit_type
			<
				int
			>,
			sge::sprite::config::float_type
			<
				float
			>
		>,
		sge::sprite::config::normal_size,
		boost::mpl::vector2
		<
			sge::sprite::config::with_color
			<
				color_format
			>,
			sge::sprite::config::with_texture
			<
				sge::sprite::config::texture_level_count
				<
					1u
				>,
				sge::sprite::config::texture_coordinates::normal
			>
		>
	> sprite_choices;

	typedef
	sge::sprite::buffers::with_declaration
	<
		sge::sprite::buffers::single
		<
			sprite_choices
		>
	>
	sprite_buffers;

	typedef
	sge::sprite::object<sprite_choices>
	sprite_object;

	typedef
	std::vector<sprite_object>
	sprite_container;

	sge::renderer::device &renderer_;
	font::color color_;
	sge::texture::manager texture_manager_;
	texture_map textures_;
	sprite_buffers sprite_buffers_;
	sprite_container sprites_;
	sge::font::rect bounding_rect_;
	font::drawer::transform_callback transform_callback_;

	sge::texture::const_part_ptr const
	cached_texture(
		sge::font::text::char_type,
		sge::font::const_image_view const &);

};
}
}
}

#endif
