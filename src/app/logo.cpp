#include "logo.hpp"
#include "../json/find_member.hpp"
#include "../json/parse_animation.hpp"
#include "../json/parse_color.hpp"
#include "../media_path.hpp"
#include <sge/viewport/manager.hpp>
#include <sge/image2d/image2d.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/active_target.hpp>
#include <sge/renderer/target_base.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/texture/texture.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/parse/json/json.hpp>
#include <mizuiro/color/operators.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>

fruitcut::app::logo::logo(
	sge::renderer::device &_renderer,
	sge::viewport::manager &_viewport_manager,
	sge::image2d::multi_loader &_image_loader,
	sge::parse::json::object const &_config_file)
:
	renderer_(
		_renderer),
	sprite_system_(
		_renderer),
	sprite_object_(
		sge::sprite::default_parameters<sprite_choices>()
			.texture(
				fcppt::make_shared_ptr<sge::texture::part_raw>(
					sge::renderer::texture::create_planar_from_view(
						renderer_,
						_image_loader.load(
							media_path() 
							/ FCPPT_TEXT("textures") 
							/ 
								json::find_member<fcppt::string>(
									_config_file,
									FCPPT_TEXT("textures/logo")))->view(),
						sge::renderer::texture::filter::linear,
						sge::renderer::texture::address_mode2(
							sge::renderer::texture::address_mode::clamp),
						sge::renderer::resource_flags::none)))
			.texture_size()
			.system(
				&sprite_system_)
			.elements()),
	viewport_change_connection_(
		_viewport_manager.manage_callback(
			std::tr1::bind(
				&logo::viewport_change,
				this))),
	color_animation_(
		json::parse_animation<color_animation>(
			json::find_member<sge::parse::json::array>(
				_config_file,
				FCPPT_TEXT("intro/logo-animation")),
			&json::parse_color<sprite_object::color_type>))
{
	viewport_change();
}

void
fruitcut::app::logo::update()
{
	color_animation_.update();
	sprite_object_.color(
		color_animation_.current_value());
}

void
fruitcut::app::logo::render()
{
	sprite_system_.render_all(
		sge::sprite::default_equal());
}

void
fruitcut::app::logo::viewport_change()
{
	sge::sprite::center(
		sprite_object_,
		fcppt::math::dim::structure_cast<sprite_object::vector>(
			sge::renderer::active_target(
				renderer_).viewport().get().size())/2);
}
