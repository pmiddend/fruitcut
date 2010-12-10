#include "font_drawer.hpp"
#include <sge/texture/rect_fragmented.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/caps.hpp>
#include <sge/image/view/dim.hpp>
#include <sge/image/color/any/convert.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/default_sort.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/sprite/external_system_impl.hpp>
#include <sge/sprite/render_states.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/quad.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/output.hpp>
#include <fcppt/math/vector/output.hpp>
#include <boost/spirit/home/phoenix/object/new.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <utility>
#include <iostream>

fruitcut::sandbox::font_drawer::font_drawer(
	sge::renderer::device_ptr const _rend,
	sge::image::color::any::object const &_col,
	font_transform_callback const &_transform)
:
	rend_(
		_rend),
	col_(
		_col),
	texman_(
		rend_,
		boost::phoenix::construct<sge::texture::fragmented_unique_ptr>(
			boost::phoenix::new_<sge::texture::rect_fragmented>(
				rend_,
				rend_->caps().preferred_texture_format(),
				sge::renderer::filter::linear,
				fcppt::math::dim::quad<sge::renderer::dim2>( 
					256)))),
	sys_(
		rend_),
	sprites_(),
	transform_(
		_transform)
{}

fruitcut::sandbox::font_drawer::~font_drawer()
{}

void
fruitcut::sandbox::font_drawer::begin_rendering(
	sge::font::text::drawer::size_type const buffer_chars,
	sge::font::pos const &origin,
	sge::font::dim const &size)
{
	sprites_.clear();
	sprites_.reserve(
		static_cast<sprite_container::size_type>(
			buffer_chars));
	bounding_pos_ = origin;
	bounding_size_ = size;
}

void
fruitcut::sandbox::font_drawer::draw_char(
	sge::font::text::char_type const _char,
	sge::font::pos const &_pos,
	sge::font::const_image_view const &_data
)
{
	sge::image::dim_type const dim(
		sge::image::view::dim(
			_data));

	typedef 
	sge::sprite::parameters<sprite_choices> 
	sprite_parameters;

	std::pair<sge::font::pos,sge::font::dim> const transformation = 
		transform_(
			bounding_pos_,
			bounding_size_,
			_pos,
			fcppt::math::dim::structure_cast<sge::font::dim>(
				dim));

	sprites_.push_back(
		sprite_object(
			sprite_parameters()
			.pos(
				fcppt::math::vector::structure_cast<sprite_object::point>(
					transformation.first))
			.texture(
				dim.content()
				?
					cached_texture(
						_char,
						_data
					)
				:
					sge::texture::const_part_ptr())
			.size(
				fcppt::math::dim::structure_cast<sprite_object::dim>(
					transformation.second))
			.color(
				// TODO:
				sge::image::color::any::convert<sprite_object::color_format>( 
					col_))
			.elements()));
}

void
fruitcut::sandbox::font_drawer::end_rendering()
{
	sys_.render(
		sprites_.begin(),
		sprites_.end(),
		sge::sprite::default_sort(),
		sge::sprite::default_equal());
}

void
fruitcut::sandbox::font_drawer::color(
	sge::image::color::any::object const &_col)
{
	col_ = _col;
}

sge::texture::const_part_ptr const
fruitcut::sandbox::font_drawer::cached_texture(
	sge::font::text::char_type const _ch,
	sge::font::const_image_view const &_data)
{
	texture_map::const_iterator const it(
		textures_.find(
			_ch));

	return
		it != textures_.end()
		?
			it->second
		:
			textures_.insert(
				std::make_pair(
					_ch,
					texman_.add(
						_data))).first->second;
}
