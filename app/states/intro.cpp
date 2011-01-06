#include "intro.hpp"
#include "../../particle/sprite/choices.hpp"
#include "../../particle/sprite/animation.hpp"
#include "../../particle/point_sprite/object.hpp"
#include "../../particle/objects/permanent_sprite.hpp"
#include "../../particle/objects/simple.hpp"
#include "../../particle/objects/unique_base_ptr.hpp"
#include "../../json/find_member.hpp"
#include "../../media_path.hpp"
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/defaults.hpp>
#include <sge/sprite/default_parameters.hpp>
#include <sge/image/color/init.hpp>
#include <sge/image2d/file_ptr.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/time/second_f.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/device.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/time/second.hpp>
#include <sge/time/activation_state.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <iostream>

fruitcut::app::states::intro::intro(
	my_context ctx)
:
	my_base(
		ctx),
	saturation_timer_(
		sge::time::second(
			3),
		sge::time::activation_state::active,
		context<machine>().timer_callback())
{
	context<machine>().desaturate_filter().factor(
		static_cast<sge::renderer::scalar>(
			0.0));

	context<machine>().particle_system().insert(
		particle::objects::unique_base_ptr(
			new particle::objects::permanent_sprite(
				sge::sprite::default_parameters<particle::sprite::choices>()
					.repetition(
						json::find_member<particle::sprite::object::repetition_type>(
							context<machine>().config_file(),
							FCPPT_TEXT("background-repeat")))
					.size(
						fcppt::math::dim::structure_cast<particle::sprite::object::dim>(
							context<machine>().systems().renderer()->screen_size()))
					// Texture is too big, can't use the texture manager here. Also the texture has repetition
					.texture(
						context<machine>().create_single_texture(	
							media_path()
								/ FCPPT_TEXT("textures")
								/ 
									json::find_member<fcppt::string>(
										context<machine>().config_file(),
										FCPPT_TEXT("textures/background"))))
					.pos(
						particle::sprite::object::point::null())
					.system(
						&context<machine>().particle_system().sprite_system()))));

	sge::image2d::file_ptr const logo_image = 
		context<machine>().systems().image_loader().load(
			fruitcut::media_path() 
				/ FCPPT_TEXT("textures") 
				/ FCPPT_TEXT("logo.png"));

	particle::point_sprite::object::point const logo_pos = 
		fcppt::math::dim::structure_cast<particle::point_sprite::object::point>(
			context<machine>().systems().renderer()->screen_size())/
				static_cast<particle::point_sprite::object::point::value_type>(
					2) - 
		fcppt::math::dim::structure_cast<particle::point_sprite::object::point>(
			logo_image->dim())/
				static_cast<particle::point_sprite::object::point::value_type>(
					2);

	// The logo
	context<machine>().particle_system().insert(
		fruitcut::particle::objects::unique_base_ptr(
			new fruitcut::particle::objects::simple<fruitcut::particle::sprite::choices>(
				sge::sprite::default_parameters<fruitcut::particle::sprite::choices>()
					.texture(
						sge::texture::part_ptr(
							// Can't use machine::create_single_texture here since
							// we've loaded the image beforehand
							new sge::texture::part_raw(
								context<machine>().systems().renderer()->create_texture(
									logo_image->view(),
									sge::renderer::filter::linear,
									sge::renderer::resource_flags::none))))
					.texture_size()
					.center(
						fcppt::math::dim::structure_cast<fruitcut::particle::sprite::object::point>(
							context<machine>().systems().renderer()->screen_size())/2)
					.system(
						&context<machine>().particle_system().sprite_system()),
				fcppt::assign::make_container<particle::sprite::animation::value_sequence>
					(particle::sprite::animation::value_type(
						sge::time::second_f(
							static_cast<sge::time::funit>(
								3)),
						fruitcut::particle::sprite::object::color_type(
							(sge::image::color::init::red %= 1.0)
							(sge::image::color::init::green %= 1.0)
							(sge::image::color::init::blue %= 1.0)
							(sge::image::color::init::alpha %= 1.0))))
					(particle::sprite::animation::value_type(
						sge::time::second_f(
							static_cast<sge::time::funit>(
								0)),
						fruitcut::particle::sprite::object::color_type(
							(sge::image::color::init::red %= 1.0)
							(sge::image::color::init::green %= 1.0)
							(sge::image::color::init::blue %= 1.0)
							(sge::image::color::init::alpha %= 0.0)))),
				sge::renderer::vector2::null(),
				sge::renderer::vector2::null())));
}

boost::statechart::result
fruitcut::app::states::intro::react(
	events::render const &)
{
	context<machine>().particle_system().render();
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::intro::react(
	events::tick const &)
{
	context<machine>().desaturate_filter().factor(
		static_cast<sge::renderer::scalar>(
			saturation_timer_.expired()
			?
				static_cast<sge::renderer::scalar>(1.0)
			:
				saturation_timer_.elapsed_frames()));
	return discard_event();
}

fruitcut::app::states::intro::~intro()
{
}
