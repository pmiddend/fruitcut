#include "intro.hpp"
#include "../../particle/sprite/parameters.hpp"
#include "../../particle/objects/permanent_sprite.hpp"
#include "../../json/find_member.hpp"
#include "../../media_path.hpp"
#include <sge/sprite/parameters_impl.hpp>
#include <sge/image/color/init.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <iostream>

fruitcut::app::states::intro::intro(
	my_context ctx)
:
	my_base(
		ctx)
{
	context<machine>().particle_system().insert(
		particle::objects::unique_base_ptr(
			new particle::objects::permanent_sprite(
				particle::sprite::parameters()
					.visible(
						true)
					.repetition(
						json::find_member<particle::sprite::object::repetition_type>(
							context<machine>().config_file(),
							FCPPT_TEXT("background-repeat")))
					.rotation(
						static_cast<particle::sprite::object::rotation_type>(
							0))
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
						&context<machine>().particle_system().sprite_system())
					.color(
						fruitcut::particle::sprite::object::color_type(
							(sge::image::color::init::red %= 1.0)
							(sge::image::color::init::green %= 1.0)
							(sge::image::color::init::blue %= 1.0)
							(sge::image::color::init::alpha %= 1.0))))));
}

boost::statechart::result
fruitcut::app::states::intro::react(
	events::render const &)
{
	context<machine>().particle_system().render();
	return discard_event();
}

fruitcut::app::states::intro::~intro()
{
}
