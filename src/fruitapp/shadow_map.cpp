#include <sge/renderer/clear/parameters.hpp>
#include <sge/renderer/target.hpp>
#include <fruitapp/shadow_map.hpp>
#include <fruitlib/perspective_projection_information_to_matrix.hpp>
#include <fruitlib/json/parse_projection.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/format.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/target.hpp>
#include <sge/renderer/target_from_texture.hpp>
#include <sge/renderer/projection/perspective_af.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/color.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/float.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/texture/capabilities.hpp>
#include <sge/renderer/texture/capabilities_field.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/planar_parameters.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>


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
				sge::parse::json::find_and_convert_member<sge::renderer::dim2>(
					_config,
					sge::parse::json::path(
						FCPPT_TEXT("shadow-map"))
						/ FCPPT_TEXT("size")),
				sge::image::color::format::rgb32f,
				sge::renderer::texture::mipmap::off(),
				sge::renderer::resource_flags::none,
				sge::renderer::texture::capabilities_field(
					sge::renderer::texture::capabilities::render_target)))),
	target_(
		sge::renderer::target_from_texture(
			renderer_,
			*texture_)),
	mvp_(
		fruitlib::perspective_projection_information_to_matrix(
			fruitlib::json::parse_projection(
				sge::parse::json::find_and_convert_member<sge::parse::json::object>(
					_config,
					sge::parse::json::path(
						FCPPT_TEXT("shadow-map"))
						/ FCPPT_TEXT("projection")),
				fcppt::optional<sge::renderer::scalar>())) *
		_modelview)
{
	// Do an initial clear of the texture to prevent race conditions
	// (the shadow map might be rendered before its first update)
	fruitlib::scenic::events::update event;

	this->react(
		event);
}

fruitapp::shadow_map_texture const
fruitapp::shadow_map::texture()
{
	return
		fruitapp::shadow_map_texture(
			texture_);
}

fruitapp::shadow_mvp const
fruitapp::shadow_map::mvp() const
{
	return
		fruitapp::shadow_mvp(
			mvp_);
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
			(sge::renderer::state::depth_func::less));

	target_->clear(
		sge::renderer::clear::parameters()
			.back_buffer(
				sge::renderer::clear::back_buffer_value(
					sge::image::colors::black()))
			.depth_buffer(
				sge::renderer::clear::depth_buffer_value(
					1.0f)));

	sge::renderer::scoped_target target(
		renderer_,
		*target_);

	sge::renderer::scoped_block block(
		renderer_);

	fruitlib::scenic::events::render event;

	node_base::forward_to_children(
		event);
}
