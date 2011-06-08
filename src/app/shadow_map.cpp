#include "shadow_map.hpp"
#include "../fruitlib/json/find_member.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/state/state.hpp>
#include <sge/renderer/target_from_texture.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/texture/texture.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/image/color/format.hpp>
#include <sge/image/colors.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/text.hpp>
#include <iostream>

fruitcut::app::shadow_map::shadow_map(
	sge::parse::json::object const &_config_file,
	sge::renderer::device &_renderer,
	sge::camera::object const &_camera)
:
	renderer_(
		_renderer),
	camera_(
		_camera),
	texture_(
		renderer_.create_planar_texture(
			sge::renderer::texture::planar_parameters(
				fruitlib::json::find_member<sge::renderer::dim2>(
					_config_file,
					FCPPT_TEXT("shadow-map/size")),
				sge::image::color::format::rgb32f,
				sge::renderer::texture::filter::linear,
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::clamp),
				sge::renderer::resource_flags::none,
				sge::renderer::texture::capabilities_field(
					sge::renderer::texture::capabilities::render_target)))),
	target_(
		sge::renderer::target_from_texture(
			renderer_,
			*texture_)),
	mvp_(
		fruitlib::json::find_member<sge::renderer::matrix4>(
			_config_file,
			FCPPT_TEXT("shadow-map/mvp")))
{
}

sge::renderer::texture::planar_ptr const
fruitcut::app::shadow_map::texture()
{
	return texture_;
}

sge::renderer::matrix4 const &
fruitcut::app::shadow_map::mvp() const
{
	return mvp_;
}

fruitcut::app::shadow_map::~shadow_map()
{
}

void
fruitcut::app::shadow_map::render()
{
}

void
fruitcut::app::shadow_map::update()
{
	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::bool_::clear_depth_buffer = true)
			(sge::renderer::state::bool_::clear_back_buffer = true)
			(sge::renderer::state::color::back_buffer_clear_color = sge::image::colors::black()));

	sge::renderer::scoped_target target(
		renderer_,
		*target_);

	sge::renderer::scoped_block block(
		renderer_);

	intrusive_group::render();
}
