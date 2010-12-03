#include "mouse_trailer.hpp"
#include "../media_path.hpp"
#include <sge/sprite/parameters_impl.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/input/mouse/device.hpp>
#include <sge/input/mouse/axis_event.hpp>
#include <sge/time/millisecond.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/init.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image/file.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/texture/part.hpp>
#include <sge/sprite/default_equal.hpp>
#include <fcppt/text.hpp>
#include <fcppt/chrono/duration_cast.hpp>
#include <fcppt/chrono/seconds.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
#include <boost/bind.hpp>
#include <iostream>

fruitcut::sandbox::mouse_trailer::mouse_trailer(
	sge::renderer::device_ptr const renderer,
	sge::image::multi_loader &image_loader,
	sge::input::mouse::device_ptr mouse)
:
	ss_(
		renderer),
	cursor_(
		sprite::parameters()
			.texture_size()
			.visible(
				true)
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						renderer->create_texture(
							image_loader.load(
								media_path() 
									/ FCPPT_TEXT("textures") 
									/ FCPPT_TEXT("cursor.png"))->view(),
							sge::renderer::filter::linear,
							sge::renderer::resource_flags::none))))
			.center(
				sprite::object::point(
					static_cast<sprite::object::unit>(
						renderer->screen_size().w()/2),
					static_cast<sprite::object::unit>(
						renderer->screen_size().h()/2)))
			.system(
				&ss_)
			.color(
				sprite::object::color_type(
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
			renderer->create_texture(
				image_loader.load(
					media_path() 
						/ FCPPT_TEXT("textures") 
						/ FCPPT_TEXT("particle.png"))->view(),
				sge::renderer::filter::linear,
				sge::renderer::resource_flags::none))),
	update_timer_(
		sge::time::millisecond(
			10)),
	old_position_(
		fcppt::math::vector::structure_cast<sge::renderer::vector2>(
			cursor_.pos() + cursor_.size()/2))
{
}

void
fruitcut::sandbox::mouse_trailer::update()
{
	if (update_timer_.update_b())
	{
		sge::renderer::vector2 const
			new_position = 
				fcppt::math::vector::structure_cast<sge::renderer::vector2>(
					cursor_.pos() + cursor_.size()/2),
			direction = 
				new_position - old_position_;

		sge::renderer::scalar const
			particle_length = 
				static_cast<sge::renderer::scalar>(
					particle_texture_->dim().w()) / 3,
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
				particle(
					sprite::parameters()
						.texture_size()
						.visible(
							true)
						.texture(
							particle_texture_)
						.center(
							fcppt::math::vector::structure_cast<fruitcut::sprite::object::point>(
								old_position_ + lambda * direction))
						.system(
							&ss_)
						.color(
							sprite::object::color_type(
								(sge::image::color::init::red %= 1.0)
								(sge::image::color::init::green %= 1.0)
								(sge::image::color::init::blue %= 1.0)
								(sge::image::color::init::alpha %= 1.0))),
					fcppt::chrono::duration_cast<particle::duration>(
						fcppt::chrono::seconds(
							1))));
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
