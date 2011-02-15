#include "mouse_trailer.hpp"
#include "../media_path.hpp"
#include <sge/sprite/parameters_impl.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/texture/create_planar_from_view.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/input/mouse/device.hpp>
#include <sge/input/mouse/axis_event.hpp>
#include <sge/time/millisecond.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/init.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/texture/part.hpp>
#include <sge/sprite/default_equal.hpp>
#include <fcppt/text.hpp>
#include <fcppt/chrono/duration_cast.hpp>
#include <fcppt/chrono/milliseconds.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/atan2.hpp>
#include <fcppt/math/range_compare.hpp>
#include <boost/bind.hpp>
#include <iostream>

fruitcut::sandbox::mouse_trailer::mouse_trailer(
	sge::renderer::device_ptr const renderer,
	sge::image2d::multi_loader &image_loader,
	sge::input::mouse::device_ptr mouse)
:
	ss_(
		renderer),
	cursor_(
		particle::sprite::parameters()
			.texture_size()
			.repetition(
				static_cast<particle::sprite::object::repetition_type>(
					1))
			.rotation(
				static_cast<particle::sprite::object::rotation_type>(
					0))
			.visible(
				true)
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						sge::renderer::texture::create_planar_from_view(
							renderer,
							image_loader.load(
								media_path() 
									/ FCPPT_TEXT("textures") 
									/ FCPPT_TEXT("cursor.png"))->view(),
							sge::renderer::texture::filter::linear,
							sge::renderer::texture::address_mode2(
								sge::renderer::texture::address_mode::clamp),
							sge::renderer::resource_flags::none))))
			.center(
				particle::sprite::object::point(
					static_cast<particle::sprite::object::unit>(
						renderer->screen_size().w()/2),
					static_cast<particle::sprite::object::unit>(
						renderer->screen_size().h()/2)))
			.system(
				&ss_)
			.color(
				particle::sprite::object::color_type(
					(sge::image::color::init::red %= 1.0)
					(sge::image::color::init::green %= 1.0)
					(sge::image::color::init::blue %= 1.0)
					(sge::image::color::init::alpha %= 1.0))).elements()),
	mouse_axis_connection_(
		mouse->axis_callback(
			boost::bind(
				&mouse_trailer::callback,
				this,
				_1))),
	particle_texture_(
		new sge::texture::part_raw(
			sge::renderer::texture::create_planar_from_view(
				renderer,
				image_loader.load(
					media_path() 
						/ FCPPT_TEXT("textures") 
						/ FCPPT_TEXT("particle.png"))->view(),
				sge::renderer::texture::filter::linear,
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::clamp),
				sge::renderer::resource_flags::none))),
	update_timer_(
		sge::time::millisecond(
			50)),
	output_timer_(
		sge::time::millisecond(
			1000)),
	old_position_(
		fcppt::math::vector::structure_cast<sge::renderer::vector2>(
			cursor_.pos() + cursor_.size()/2))
{
}

void
fruitcut::sandbox::mouse_trailer::update()
{
	if (output_timer_.update_b())
		std::cout << "Current number of sprites: " << particles_.size() << "\n";	

	if (update_timer_.update_b())
	{
		sge::renderer::vector2 const
			new_position = 
				fcppt::math::vector::structure_cast<sge::renderer::vector2>(
					cursor_.pos() + cursor_.size()/2),
			direction = 
				new_position - old_position_;

		if (!fcppt::math::range_compare(old_position_,new_position,static_cast<sge::renderer::scalar>(0.001)))
		{
			sge::renderer::scalar const
				particle_length = 
					static_cast<sge::renderer::scalar>(
						particle_texture_->dim().w()/5),
				length = 
					std::max(
						fcppt::math::vector::length(
							direction),
						particle_length + 1);

			for(
				sge::renderer::scalar lambda = 
					static_cast<sge::renderer::scalar>(0); 
				lambda < static_cast<sge::renderer::scalar>(1);
				lambda += particle_length/length)
			{
				particles_.push_back(
					sword_particle(
						particle::sprite::parameters()
							.texture_size()
							.visible(
								true)
							.texture(
								particle_texture_)
							.repetition(
								static_cast<fruitcut::particle::sprite::object::repetition_type>(
									1))
							.rotation(
								fcppt::math::vector::atan2(
									direction))
							.center(
								fcppt::math::vector::structure_cast<fruitcut::particle::sprite::object::point>(
									old_position_ + lambda * direction))
							.system(
								&ss_)
							.color(
								particle::sprite::object::color_type(
									(sge::image::color::init::red %= 1.0)
									(sge::image::color::init::green %= 1.0)
									(sge::image::color::init::blue %= 1.0)
									(sge::image::color::init::alpha %= 1.0))),
						fcppt::chrono::duration_cast<sword_particle::duration>(
							fcppt::chrono::milliseconds(
								200))));
			}
		}
	
		old_position_ = 
			fcppt::math::vector::structure_cast<sge::renderer::vector2>(
				cursor_.pos() + cursor_.size()/2);
	}

	for (particle_sequence::iterator i = particles_.begin(); i != particles_.end();)
	{
		i->update();
		if (i->dead())
			i = 
				particles_.erase(
					i);
		else
			++i;
	}
}

void
fruitcut::sandbox::mouse_trailer::render()
{
	ss_.render_all(
		sge::sprite::default_equal());
}

fruitcut::sandbox::mouse_trailer::~mouse_trailer()
{
}

void
fruitcut::sandbox::mouse_trailer::callback(
	sge::input::mouse::axis_event const &e)
{
	cursor_.pos(
		cursor_.pos() + 
		particle::sprite::object::point(
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

