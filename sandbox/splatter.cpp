#include "splatter.hpp"
#include "../media_path.hpp"
#include "../sprite/parameters.hpp"
#include "../sprite/object.hpp"
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/image/file.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/input/mouse/device.hpp>
#include <sge/input/mouse/axis_event.hpp>
#include <sge/input/mouse/button_event.hpp>
#include <sge/input/mouse/axis.hpp>
#include <sge/image/color/init.hpp>
#include <sge/time/funit.hpp>
#include <sge/time/second_f.hpp>
#include <fcppt/random/make_inclusive_range.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/math/vector/atan2.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/filesystem/directory_iterator.hpp>
#include <fcppt/text.hpp>
#include <boost/bind.hpp>
#include <boost/next_prior.hpp>
#include <iostream>
#include <cmath>

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
	texture_rng_(
		fcppt::random::make_inclusive_range(
			static_cast<texture_vector::size_type>(0),
			static_cast<texture_vector::size_type>(
				textures_.size()-1))),
	color_rng_(
		fcppt::random::make_inclusive_range(
			0.0,
			1.0)),
	// NOTE: This is in _degrees_
	rotation_rng_(
		fcppt::random::make_inclusive_range(
			static_cast<sge::renderer::scalar>(
				-30.0),
			static_cast<sge::renderer::scalar>(
				30.0))),
	speed_range_(
		std::make_pair(
			static_cast<sge::renderer::scalar>(100),
			static_cast<sge::renderer::scalar>(1000))),
	// And this is in pixels per second
	speed_rng_(
		fcppt::random::make_inclusive_range(
			speed_range_.first,
			speed_range_.second)),
	// And this is in seconds
	lifetime_rng_(
		fcppt::random::make_inclusive_range(
			static_cast<sge::time::funit>(
				0.1),
			static_cast<sge::time::funit>(
				1.5))),
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
	splat_collector_.insert(
		sprite::parameters()
			.size(
				fcppt::math::dim::structure_cast<sprite::object::dim>(
					_renderer->screen_size()))
			.order(
				static_cast<sprite::object::order_type>(
					0))
			.visible(
				true)
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						_renderer->create_texture(
							_image_loader.load(
								media_path() 
									/ FCPPT_TEXT("textures") 
									/ FCPPT_TEXT("wood.png"))->view(),
							sge::renderer::filter::linear,
							sge::renderer::resource_flags::none))))
			.pos(
				sprite::object::point::null())
			.color(
				sprite::object::color_type(
					(sge::image::color::init::red %= 1.0)
					(sge::image::color::init::green %= 1.0)
					(sge::image::color::init::blue %= 1.0)
					(sge::image::color::init::alpha %= 1.0))));
}

void
fruitcut::sandbox::splatter::update()
{
	for (particle_list::iterator i = particles_.begin(); i != particles_.end();)
	{
		i->update();
		if (i->dead())
		{
			splat_collector_.insert(
				sprite::parameters()
					.size(
						i->sprite().size())
					.order(
						static_cast<sprite::object::order_type>(
							0))
					.rotation(
						i->sprite().rotation())
					.visible(
						true)
					.texture(
						i->sprite().texture())
					.pos(
						i->sprite().pos())
					.color(
						i->sprite().color()));
			i = particles_.erase(i);
		}
		else
			++i;
	}

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
	if (e.pressed())
	{
		first_position_ = cursor_.pos() + cursor_.size()/2;
		return;
	}

	unsigned const particle_count = 
		200;

	sge::renderer::vector2 const direction = 
		fcppt::math::vector::normalize(
			fcppt::math::vector::structure_cast<sge::renderer::vector2>(
				(cursor_.pos() + cursor_.size()/2) - first_position_));

	for (unsigned i = 0; i < particle_count; ++i)
	{
		sge::renderer::scalar const rotation = 
			fcppt::math::deg_to_rad(
				rotation_rng_());
	
		sge::renderer::vector2 const rotated(
			direction.x() * std::cos(rotation) - direction.y() * std::sin(rotation),	
			direction.y() * std::cos(rotation) + direction.x() * std::sin(rotation));

		sge::renderer::scalar const speed_scalar = 
			speed_rng_();

		sge::renderer::vector2 const 
			speed = 
				speed_scalar * rotated,
			acceleration = 
				static_cast<sge::renderer::scalar>(-0.8) * speed;

		sge::renderer::texture_ptr const texture = 
			*boost::next(
				textures_.begin(),
				texture_rng_());
		
		particles_.push_back(
			new splat_particle(
				sprite::parameters()
					.size(
						sprite::object::dim(
							static_cast<sprite::object::unit>(
								texture->dim().w()),
							static_cast<sprite::object::unit>(
								static_cast<sge::renderer::scalar>(texture->dim().h()) * 
									std::max(
										static_cast<sge::renderer::scalar>(
											0.2),
										(static_cast<sge::renderer::scalar>(1.0) - 
											(speed_scalar - speed_range_.first) / 
												(speed_range_.second - speed_range_.first))))
							))
					.order(
						static_cast<sprite::object::order_type>(
							-101))
					.visible(
						true)
					.rotation(
						fcppt::math::vector::atan2(
							rotated))
					.texture(
						sge::texture::part_ptr(
							new sge::texture::part_raw(
								texture)))
					.center(
						cursor_.pos() + cursor_.size()/2)
					.system(
						&ss_)
					.color(
						/*
						sprite::object::color_type(
							(sge::image::color::init::red %= 1.0)
							(sge::image::color::init::green %= 1.0)
							(sge::image::color::init::blue %= 1.0)
							(sge::image::color::init::alpha %= 1.0))*/
						sprite::object::color_type(
							(sge::image::color::init::red %= color_rng_())
							(sge::image::color::init::green %= color_rng_())
							(sge::image::color::init::blue %= color_rng_())
							(sge::image::color::init::alpha %= 0.1))),
				sge::time::second_f(
					lifetime_rng_()),
				speed,
				acceleration));
	}
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
