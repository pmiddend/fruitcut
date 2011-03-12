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
#include <sge/image2d/file.hpp>
#include <sge/image2d/file_ptr.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image/color/init.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/renderer/texture/create_planar_from_view.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/sprite/default_parameters.hpp>
#include <sge/sprite/defaults.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/time/activation_state.hpp>
#include <sge/time/funit.hpp>
#include <sge/time/second_f.hpp>
#include <sge/time/second.hpp>
#include <sge/viewport/manager.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/move.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <iostream>

fruitcut::app::states::intro::intro(
	my_context ctx)
:
	my_base(
		ctx),
	// Those timers will be activated as soon as we have a viewport
	saturation_timer_(
		sge::time::second_f(
			json::find_member<sge::time::funit>(
				context<machine>().config_file(),
				FCPPT_TEXT("intro/desaturation-secs"))),
		sge::time::activation_state::inactive,
		context<machine>().timer_callback()),
	intro_timer_(
		sge::time::second_f(
			json::find_member<sge::time::funit>(
				context<machine>().config_file(),
				FCPPT_TEXT("intro/total-duration-secs"))),
		sge::time::activation_state::inactive,
		context<machine>().timer_callback()),
	viewport_change_connection_(
		context<machine>().systems().viewport_manager().manage_callback(
			std::tr1::bind(
				&intro::viewport_change,
				this)))
{
	// We already have a viewport? Ok, then go
	if(context<machine>().systems().renderer()->onscreen_target()->viewport().get().size().content())
		viewport_change();
}

boost::statechart::result
fruitcut::app::states::intro::react(
	events::render const &)
{
	context<machine>().background().render();
	context<machine>().particle_system().render();
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::intro::react(
	events::tick const &)
{
	context<machine>().sound_controller().update();
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

void
fruitcut::app::states::intro::viewport_change()
{
	if (saturation_timer_.active())
		return;

	saturation_timer_.activate();
	intro_timer_.activate();

	context<machine>().sound_controller().play(
		FCPPT_TEXT("intro"));

	sge::image2d::file_ptr const logo_image = 
		context<machine>().systems().image_loader().load(
			media_path() 
				/ FCPPT_TEXT("textures") 
				/ 
					json::find_member<fcppt::string>(
						context<machine>().config_file(),
						FCPPT_TEXT("textures/logo")));

	particle::point_sprite::object::vector const logo_pos = 
		fcppt::math::dim::structure_cast<particle::point_sprite::object::vector>(
			context<machine>().systems().renderer()->onscreen_target()->viewport().get().size())/
				static_cast<particle::point_sprite::object::vector::value_type>(
					2) - 
		fcppt::math::dim::structure_cast<particle::point_sprite::object::vector>(
			logo_image->size())/
				static_cast<particle::point_sprite::object::vector::value_type>(
					2);

	// The logo
	context<machine>().particle_system().insert(
		fruitcut::particle::objects::unique_base_ptr(
		fcppt::make_unique_ptr<fruitcut::particle::objects::simple<fruitcut::particle::sprite::choices> >(
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
					fcppt::math::dim::structure_cast<fruitcut::particle::sprite::object::vector>(
						context<machine>().systems().renderer()->onscreen_target()->viewport().get().size())/2)
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
			fcppt::make_unique_ptr<fruitcut::particle::objects::simple<fruitcut::particle::sprite::choices> >(
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
						fcppt::math::dim::structure_cast<fruitcut::particle::sprite::object::vector>(
							context<machine>().systems().renderer()->onscreen_target()->viewport().get().size())/2 + 
						json::find_member<fruitcut::particle::sprite::object::vector>(
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
