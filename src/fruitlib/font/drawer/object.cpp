#include <fruitlib/font/drawer/object.hpp>
#include <fruitlib/font/drawer/parameters.hpp>
#include <sge/image2d/dim.hpp>
#include <sge/image2d/view/size.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/caps/object.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/buffers/option.hpp>
#include <sge/sprite/buffers/single_impl.hpp>
#include <sge/sprite/buffers/with_declaration_impl.hpp>
#include <sge/sprite/compare/default.hpp>
#include <sge/sprite/geometry/make_random_access_range.hpp>
#include <sge/sprite/process/all.hpp>
#include <sge/texture/fragmented_unique_ptr.hpp>
#include <sge/texture/rect_fragmented.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/spirit/home/phoenix/object.hpp>
#include <iostream>
#include <utility>
#include <fcppt/config/external_end.hpp>


fruitlib::font::drawer::object::object(
	parameters const &params)
:
	renderer_(
		params.renderer()),
	color_(
		params.color()),
	texture_manager_(
		boost::phoenix::construct<sge::texture::fragmented_unique_ptr>(
			boost::phoenix::new_<sge::texture::rect_fragmented>(
				fcppt::ref(
					renderer_),
				// Is this safe? Could be a non-alpha format
				renderer_.caps().preferred_texture_format().get(),
				sge::renderer::texture::mipmap::off(),
				sge::renderer::dim2(
					256,
					256)))),
	textures_(),
	sprite_buffers_(
		renderer_,
		sge::sprite::buffers::option::dynamic),
	sprites_(),
	bounding_rect_(),
	transform_callback_(
		params.transform_callback())
{}

fruitlib::font::drawer::object::~object()
{}

void
fruitlib::font::drawer::object::begin_rendering(
	sge::font::text::drawer::size_type const buffer_chars,
	sge::font::pos const &origin,
	sge::font::dim const &size)
{
	sprites_.clear();
	sprites_.reserve(
		static_cast<sprite_container::size_type>(
			buffer_chars));
	bounding_rect_ =
		sge::font::rect(
			origin,
			size);
}

void
fruitlib::font::drawer::object::draw_char(
	sge::font::text::char_type const _char,
	sge::font::pos const &_pos,
	sge::font::const_image_view const &_data)
{
	sge::image2d::dim const d(
		sge::image2d::view::size(
			_data));

	typedef
	sge::sprite::parameters<sprite_choices>
	sprite_parameters;

	sge::font::rect const transformation =
		transform_callback_(
			bounding_rect_,
			sge::font::rect(
				_pos,
				fcppt::math::dim::structure_cast<sge::font::dim>(
					d)));

	sprites_.push_back(
		sprite_object(
			sprite_parameters()
			.pos(
				fcppt::math::vector::structure_cast<sprite_object::vector>(
					transformation.pos()))
			.texture(
				d.content()
				?
					cached_texture(
						_char,
						_data)
				:
					sge::texture::const_part_shared_ptr())
			.size(
				fcppt::math::dim::structure_cast<sprite_object::dim>(
					transformation.size()))
			.color(
				color_)));
}

void
fruitlib::font::drawer::object::end_rendering()
{
	sge::sprite::process::all(
		sge::sprite::geometry::make_random_access_range(
			sprites_),
		sprite_buffers_,
		sge::sprite::compare::default_());
}

void
fruitlib::font::drawer::object::color(
	font::color const &_color)
{
	color_ =
		_color;
}

fruitlib::font::color const &
fruitlib::font::drawer::object::color() const
{
	return color_;
}

void
fruitlib::font::drawer::object::transform_callback(
	fruitlib::font::drawer::transform_callback const &_transform_callback)
{
	transform_callback_ =
		_transform_callback;
}

fruitlib::font::drawer::transform_callback const &
fruitlib::font::drawer::object::transform_callback() const
{
	return transform_callback_;
}

sge::texture::const_part_shared_ptr const
fruitlib::font::drawer::object::cached_texture(
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
					sge::texture::const_part_shared_ptr(
						texture_manager_.add(
							_data)))).first->second;
}
