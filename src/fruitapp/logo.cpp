#include "logo.hpp"
#include "../fruitlib/json/find_and_convert_member.hpp"
#include "../fruitlib/json/parse_animation.hpp"
#include "../fruitlib/json/parse_color.hpp"
#include "../fruitlib/scenic/events/render.hpp"
#include "../fruitlib/scenic/events/viewport_change.hpp"
#include "../media_path.hpp"
#include <sge/image2d/image2d.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/viewport_size.hpp>
#include <sge/renderer/target_base.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/texture/texture.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/parse/json/json.hpp>
#include <mizuiro/color/operators.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>

fruitapp::logo::logo(
	fruitlib::scenic::optional_parent const &_parent,
	sge::renderer::device &_renderer,
	sge::image2d::multi_loader &_image_loader,
	sge::parse::json::object const &_config_file)
:
	node_base(
		_parent),
	renderer_(
		_renderer),
	sprite_system_(
		_renderer),
	sprite_object_(
		sge::sprite::default_parameters<sprite_choices>()
			.texture(
				fcppt::make_shared_ptr<sge::texture::part_raw>(
					sge::renderer::texture::create_planar_from_path(
						fruitcut::media_path() 
							/ FCPPT_TEXT("textures") 
							/ 
								fruitlib::json::find_and_convert_member<fcppt::string>(
									_config_file,
									fruitlib::json::path(
										FCPPT_TEXT("textures"))
										/ FCPPT_TEXT("logo")),
						renderer_,
						_image_loader,
						sge::renderer::texture::mipmap::off(),
						sge::renderer::texture::address_mode2(
							sge::renderer::texture::address_mode::clamp),
						sge::renderer::resource_flags::none)))
			.texture_size()
			.system(
				&sprite_system_)
			.elements())
{
	react(
		fruitlib::scenic::events::viewport_change());
}

fruitapp::logo::~logo()
{
}

void
fruitapp::logo::react(
	fruitlib::scenic::events::render const &)
{
	sprite_system_.render_all(
		sge::sprite::default_equal());
}

void
fruitapp::logo::react(
	fruitlib::scenic::events::viewport_change const &)
{
	sge::renderer::screen_size const viewport_size = 
		sge::renderer::viewport_size(
			renderer_);

	sge::sprite::center(
		sprite_object_,
		sprite_object::vector(
			static_cast<sprite_object::vector::value_type>(
				viewport_size.w()/2),
			static_cast<sprite_object::vector::value_type>(
				viewport_size.h()/4)));
}
