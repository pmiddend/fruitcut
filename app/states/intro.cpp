#include "intro.hpp"
#include "running.hpp"
#include "../postprocessing.hpp"
#include "../../particle/sprite/choices.hpp"
#include "../../particle/sprite/animation.hpp"
#include "../../particle/point_sprite/object.hpp"
#include "../../particle/objects/permanent_sprite.hpp"
#include "../../particle/objects/simple.hpp"
#include "../../particle/objects/unique_base_ptr.hpp"
#include "../../json/find_member.hpp"
#include "../../json/parse_animation.hpp"
#include "../../json/parse_color.hpp"
#include "../../media_path.hpp"
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/defaults.hpp>
#include <sge/sprite/default_parameters.hpp>
#include <sge/image/color/init.hpp>
#include <sge/image2d/file_ptr.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/texture/create_planar_from_view.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/device.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/image/color/init.hpp>
#include <sge/time/second.hpp>
#include <sge/time/funit.hpp>
#include <sge/time/activation_state.hpp>
#include <sge/time/second_f.hpp>
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
		sge::time::second_f(
			json::find_member<sge::time::funit>(
				context<machine>().config_file(),
				FCPPT_TEXT("intro/desaturation-secs"))),
		sge::time::activation_state::active,
		context<machine>().timer_callback()),
	intro_timer_(
		sge::time::second_f(
			json::find_member<sge::time::funit>(
				context<machine>().config_file(),
				FCPPT_TEXT("intro/total-duration-secs"))),
		sge::time::activation_state::active,
		context<machine>().timer_callback())
{
	context<machine>().play_sound(
		FCPPT_TEXT("intro"));
	
	context<machine>().postprocessing().desaturate_filter().factor(
		static_cast<sge::renderer::scalar>(
			0.0));

	context<machine>().particle_system().insert(
		particle::objects::unique_base_ptr(
			new particle::objects::permanent_sprite(
				sge::sprite::default_parameters<particle::sprite::choices>()
					.repetition(
						json::find_member<particle::sprite::object::repetition_type>(
							context<machine>().config_file(),
							FCPPT_TEXT("intro/background-repeat")))
					.size(
						fcppt::math::dim::structure_cast<particle::sprite::object::dim>(
							context<machine>().systems().renderer()->screen_size()))
					.order(
						static_cast<particle::sprite::object::order_type>(
							-101))
					// Texture is too big, can't use the texture manager here. Also the texture has repetition
					.texture(
						context<machine>().create_single_texture(	
							media_path()
								/ FCPPT_TEXT("textures")
								/ 
									json::find_member<fcppt::string>(
										context<machine>().config_file(),
										FCPPT_TEXT("textures/background")),
							sge::renderer::texture::address_mode::repeat))
					.pos(
						particle::sprite::object::point::null())
					.system(
						&context<machine>().particle_system().sprite_system()))));

	sge::image2d::file_ptr const logo_image = 
		context<machine>().systems().image_loader().load(
			fruitcut::media_path() 
				/ FCPPT_TEXT("textures") 
				/ 
					json::find_member<fcppt::string>(
						context<machine>().config_file(),
						FCPPT_TEXT("textures/logo")));

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
								sge::renderer::texture::create_planar_from_view(
									context<machine>().systems().renderer(),
									logo_image->view(),
									sge::renderer::texture::filter::linear,
									sge::renderer::texture::address_mode2(
										sge::renderer::texture::address_mode::clamp),
									sge::renderer::resource_flags::none))))
					.texture_size()
					.order(
						static_cast<particle::sprite::object::order_type>(
							-99))
					.center(
						fcppt::math::dim::structure_cast<fruitcut::particle::sprite::object::point>(
							context<machine>().systems().renderer()->screen_size())/2)
					.system(
						&context<machine>().particle_system().sprite_system()),
				json::parse_animation<particle::sprite::animation>(
					json::find_member<sge::parse::json::array>(
						context<machine>().config_file(),
						FCPPT_TEXT("intro/logo-animation")),
					&json::parse_color<particle::sprite::object::color_type>),
				context<machine>().timer_callback(),
				sge::renderer::vector2::null(),
				sge::renderer::vector2::null())));

	// The logo shadow
	context<machine>().particle_system().insert(
		fruitcut::particle::objects::unique_base_ptr(
			new fruitcut::particle::objects::simple<fruitcut::particle::sprite::choices>(
				sge::sprite::default_parameters<fruitcut::particle::sprite::choices>()
					.texture(
						context<machine>().create_single_texture(
							fruitcut::media_path() 
								/ FCPPT_TEXT("textures") 
								/ 
									json::find_member<fcppt::string>(
										context<machine>().config_file(),
										FCPPT_TEXT("textures/logo-shadow")),
							sge::renderer::texture::address_mode::clamp))
					.texture_size()
					.order(
						static_cast<particle::sprite::object::order_type>(
							-100))
					.center(
						fcppt::math::dim::structure_cast<fruitcut::particle::sprite::object::point>(
							context<machine>().systems().renderer()->screen_size())/2 + 
						json::find_member<fruitcut::particle::sprite::object::point>(
							context<machine>().config_file(),
							FCPPT_TEXT("intro/logo-shadow-offset")))
					.system(
						&context<machine>().particle_system().sprite_system()),
				json::parse_animation<particle::sprite::animation>(
					json::find_member<sge::parse::json::array>(
						context<machine>().config_file(),
						FCPPT_TEXT("intro/logo-shadow-animation")),
					&json::parse_color<particle::sprite::object::color_type>),
				context<machine>().timer_callback(),
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
	context<machine>().particle_system().update();
	context<machine>().postprocessing().desaturate_filter().factor(
		static_cast<sge::renderer::scalar>(
			saturation_timer_.expired()
			?
				static_cast<sge::renderer::scalar>(1.0)
			:
				saturation_timer_.elapsed_frames()));
	if (intro_timer_.expired())
		return transit<running>();
	return discard_event();
}

fruitcut::app::states::intro::~intro()
{
}
