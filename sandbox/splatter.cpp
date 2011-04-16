#include "splatter.hpp"
#include "../media_path.hpp"
#include "../particle/sprite/parameters.hpp"
#include "../particle/sprite/object.hpp"
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/image2d/file.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/texture/create_planar_from_view.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/sprite/default_parameters.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/defaults.hpp>
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
#include <fcppt/tr1/functional.hpp>
#include <boost/next_prior.hpp>
#include <iostream>
#include <cmath>

fruitcut::sandbox::splatter::splatter(
	sge::renderer::device_ptr const _renderer,
	sge::image2d::multi_loader &_image_loader,
	sge::input::mouse::device &_mouse)
:
	splat_collector_(
		_renderer),
	ss_(
		_renderer),
	cursor_(
		sge::sprite::default_parameters<particle::sprite::choices>()
			.texture_size()
			.order(
				static_cast<particle::sprite::object::order_type>(
					-100))
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						sge::renderer::texture::create_planar_from_view(
							_renderer,
							_image_loader.load(
								media_path() 
									/ FCPPT_TEXT("textures") 
									/ FCPPT_TEXT("cursor.png"))->view(),
							sge::renderer::texture::filter::linear,
							sge::renderer::texture::address_mode2(
								sge::renderer::texture::address_mode::clamp),
							sge::renderer::resource_flags::none))))
			.center(
				particle::sprite::object::vector(
					static_cast<particle::sprite::object::unit>(
						_renderer->onscreen_target()->viewport().get().size().w()/2),
					static_cast<particle::sprite::object::unit>(
						_renderer->onscreen_target()->viewport().get().size().h()/2)))
			.system(
				&ss_)
			.elements()),
	canvas_(
		sge::sprite::default_parameters<particle::sprite::choices>()
			.texture_size()
			.order(
				static_cast<particle::sprite::object::order_type>(
					100))
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						splat_collector_.texture())))
			.system(
				&ss_)
			.elements()),
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
			std::tr1::bind(
				&splatter::move_callback,
				this,
				std::tr1::placeholders::_1))),
	click_connection_(
		_mouse.button_callback(
			std::tr1::bind(
				&splatter::click_callback,
				this,
				std::tr1::placeholders::_1)))
{
	splat_collector_.insert(
		sge::sprite::default_parameters<particle::sprite::choices>()
			.size(
				fcppt::math::dim::structure_cast<particle::sprite::object::dim>(
					_renderer->onscreen_target()->viewport().get().size()))
			.repetition(
				particle::sprite::object::repetition_type(
					3,
					3))
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						sge::renderer::texture::create_planar_from_view(
							_renderer,
							_image_loader.load(
								media_path() 
									/ FCPPT_TEXT("textures") 
									/ FCPPT_TEXT("wood.png"))->view(),
							sge::renderer::texture::filter::linear,
							sge::renderer::texture::address_mode2(
								sge::renderer::texture::address_mode::repeat),
							sge::renderer::resource_flags::none)))));
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
				sge::sprite::default_parameters<particle::sprite::choices>()
					.size(
						i->sprite().size())
					.rotation(
						i->sprite().rotation())
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

		sge::renderer::texture::planar_ptr const texture = 
			*boost::next(
				textures_.begin(),
				texture_rng_());
		
		particles_.push_back(
			new splat_particle(
				sge::sprite::default_parameters<particle::sprite::choices>()
					.size(
						particle::sprite::object::dim(
							static_cast<particle::sprite::object::unit>(
								texture->size().w()),
							static_cast<particle::sprite::object::unit>(
								static_cast<sge::renderer::scalar>(texture->size().h()) * 
									std::max(
										static_cast<sge::renderer::scalar>(
											0.2),
										(static_cast<sge::renderer::scalar>(1.0) - 
											(speed_scalar - speed_range_.first) / 
												(speed_range_.second - speed_range_.first))))))
					.order(
						static_cast<particle::sprite::object::order_type>(
							-101))
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
						particle::sprite::object::color_type(
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
		particle::sprite::object::vector(
			e.axis() == sge::input::mouse::axis::x 
			? 
				static_cast<particle::sprite::object::unit>(
					e.axis_value())
			:
				static_cast<particle::sprite::object::unit>(
					0),
			e.axis() == sge::input::mouse::axis::x 
			? 
				static_cast<particle::sprite::object::unit>(
					0)
			:
				static_cast<particle::sprite::object::unit>(
					e.axis_value())));
}

fruitcut::sandbox::splatter::texture_vector const
fruitcut::sandbox::splatter::load_textures(
	sge::renderer::device_ptr const renderer,
	sge::image2d::multi_loader &image_loader)
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
			sge::renderer::texture::create_planar_from_view(
				renderer,
				image_loader.load(
					*it)->view(),
				sge::renderer::texture::filter::linear,
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::clamp),
				sge::renderer::resource_flags::none));

	return t;
}
