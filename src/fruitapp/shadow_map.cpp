#include <fruitapp/shadow_map.hpp>
#include <fruitlib/json/find_and_convert_member.hpp>
#include <fruitlib/json/parse_projection.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/float.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/color.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/target_from_texture.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/texture/planar_parameters.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/renderer/texture/capabilities_field.hpp>
#include <sge/renderer/texture/capabilities.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/image/color/format.hpp>
#include <sge/image/colors.hpp>
#include <sge/camera/projection/to_matrix.hpp>
#include <sge/parse/json/object.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/chrono/duration_impl.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/text.hpp>
#include <iostream>

fruitapp::shadow_map::shadow_map(
	fruitlib::scenic::optional_parent const &_parent,
	sge::parse::json::object const &_config,
	sge::renderer::device &_renderer,
	sge::renderer::matrix4 const &_modelview)
:
	node_base(
		_parent),
	renderer_(
		_renderer),
	texture_(
		renderer_.create_planar_texture(
			sge::renderer::texture::planar_parameters(
				fruitlib::json::find_and_convert_member<sge::renderer::dim2>(
					_config,
					fruitlib::json::path(
						FCPPT_TEXT("shadow-map"))
						/ FCPPT_TEXT("size")),
				sge::image::color::format::rgb32f,
				sge::renderer::texture::mipmap::off(),
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
		sge::camera::projection::to_matrix(
			fruitlib::json::parse_projection(
				fruitlib::json::find_and_convert_member<sge::parse::json::object>(
					_config,
					fruitlib::json::path(
						FCPPT_TEXT("shadow-map"))
						/ FCPPT_TEXT("projection")),
				fcppt::optional<sge::renderer::scalar>())) * 
		_modelview)
{
	// Do an initial clear of the texture to prevent race conditions
	// (the shadow map might be rendered before its first update)
	react(
		fruitlib::scenic::events::update());
}

sge::renderer::texture::planar_ptr const
fruitapp::shadow_map::texture()
{
	return texture_;
}

sge::renderer::matrix4 const &
fruitapp::shadow_map::mvp() const
{
	return mvp_;
}

fruitapp::shadow_map::~shadow_map()
{
}


void
fruitapp::shadow_map::react(
	fruitlib::scenic::events::update const &)
{
	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::float_::depth_buffer_clear_val = 1.0f)
			(sge::renderer::state::depth_func::less)
			(sge::renderer::state::bool_::clear_depth_buffer = true)
			(sge::renderer::state::bool_::clear_back_buffer = true)
			(sge::renderer::state::color::back_buffer_clear_color = sge::image::colors::black()));

	sge::renderer::scoped_target target(
		renderer_,
		*target_);

	sge::renderer::scoped_block block(
		renderer_);

	node_base::forward_to_children(
		fruitlib::scenic::events::render());
}
