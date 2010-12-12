#include "splatter.hpp"
#include "../media_path.hpp"
#include "../sprite/parameters.hpp"
#include "../sprite/object.hpp"
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/image/file.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/input/mouse/device.hpp>
#include <sge/input/mouse/axis_event.hpp>
#include <sge/input/mouse/button_event.hpp>
#include <sge/input/mouse/axis.hpp>
#include <sge/image/color/init.hpp>
#include <fcppt/random/make_inclusive_range.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/filesystem/directory_iterator.hpp>
#include <fcppt/text.hpp>
#include <boost/bind.hpp>

fruitcut::sandbox::splatter::splatter(
	sge::renderer::device_ptr const _renderer,
	sge::image::multi_loader &_image_loader,
	sge::input::mouse::device &_mouse)
:
	splat_collector_(
		_renderer),
	ss_(
		_renderer),
	cursor_(
		sprite::parameters()
			.texture_size()
			.order(
				static_cast<sprite::object::order_type>(
					-100))
			.visible(
				true)
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						_renderer->create_texture(
							_image_loader.load(
								media_path() 
									/ FCPPT_TEXT("textures") 
									/ FCPPT_TEXT("cursor.png"))->view(),
							sge::renderer::filter::linear,
							sge::renderer::resource_flags::none))))
			.center(
				sprite::object::point(
					static_cast<sprite::object::unit>(
						_renderer->screen_size().w()/2),
					static_cast<sprite::object::unit>(
						_renderer->screen_size().h()/2)))
			.system(
				&ss_)
			.color(
				sprite::object::color_type(
					(sge::image::color::init::red %= 1.0)
					(sge::image::color::init::green %= 1.0)
					(sge::image::color::init::blue %= 1.0)
					(sge::image::color::init::alpha %= 1.0))).elements()),
	canvas_(
		sprite::parameters()
			.texture_size()
			.visible(
				true)
			.order(
				static_cast<sprite::object::order_type>(
					100))
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						splat_collector_.texture())))
			.pos(
				sprite::object::point::null())
			.system(
				&ss_)
			.color(
				sprite::object::color_type(
					(sge::image::color::init::red %= 1.0)
					(sge::image::color::init::green %= 1.0)
					(sge::image::color::init::blue %= 1.0)
					(sge::image::color::init::alpha %= 1.0))).elements()),
	textures_(
		load_textures(
			_renderer,
			_image_loader)),
	rng_(
		fcppt::random::make_inclusive_range(
			static_cast<texture_vector::size_type>(0),
			static_cast<texture_vector::size_type>(
				textures_.size()-1))),
	color_rng_(
		fcppt::random::make_inclusive_range(
			0.0,
			1.0)),
	move_connection_(
		_mouse.axis_callback(
			boost::bind(
				&splatter::move_callback,
				this,
				_1))),
	click_connection_(
		_mouse.button_callback(
			boost::bind(
				&splatter::click_callback,
				this,
				_1)))
{
}

void
fruitcut::sandbox::splatter::update()
{
	splat_collector_.update();
}

void
fruitcut::sandbox::splatter::render()
{
	ss_.render_all(
		sge::sprite::default_equal());
}

void
fruitcut::sandbox::splatter::click_callback(
	sge::input::mouse::button_event const &e)
{
	if (!e.pressed())
		return;

	splat_collector_.insert(
		sprite::parameters()
			.texture_size()
			.order(
				static_cast<sprite::object::order_type>(
					0))
			.visible(
				true)
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						*boost::next(
							textures_.begin(),
							rng_()))))
			.center(
				cursor_.pos() + cursor_.size()/2)
			.system(
				&ss_)
			.color(
				sprite::object::color_type(
					(sge::image::color::init::red %= color_rng_())
					(sge::image::color::init::green %= color_rng_())
					(sge::image::color::init::blue %= color_rng_())
					(sge::image::color::init::alpha %= 1.0))));
}

void
fruitcut::sandbox::splatter::move_callback(
	sge::input::mouse::axis_event const &e)
{
	cursor_.pos(
		cursor_.pos() + 
		sprite::object::point(
			e.axis() == sge::input::mouse::axis::x 
			? 
				static_cast<sprite::object::unit>(
					e.axis_value())
			:
				static_cast<sprite::object::unit>(
					0),
			e.axis() == sge::input::mouse::axis::x 
			? 
				static_cast<sprite::object::unit>(
					0)
			:
				static_cast<sprite::object::unit>(
					e.axis_value())));
}

fruitcut::sandbox::splatter::texture_vector const
fruitcut::sandbox::splatter::load_textures(
	sge::renderer::device_ptr const renderer,
	sge::image::multi_loader &image_loader)
{
	texture_vector t;
	for(
		fcppt::filesystem::directory_iterator it(
			media_path()
				/FCPPT_TEXT("textures")
				/FCPPT_TEXT("splatter")); 
		it != fcppt::filesystem::directory_iterator(); 
		++it)
		t.push_back(
			renderer->create_texture(
				image_loader.load(
					*it)->view(),
				sge::renderer::filter::linear,
				sge::renderer::resource_flags::none));

	return t;
}
