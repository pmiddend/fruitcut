#include <fruitapp/light/directional_light_source.hpp>
#include <fruitapp/light/manager.hpp>
#include <fruitapp/shadow_map/object.hpp>
#include <fruitlib/perspective_projection_information_to_matrix.hpp>
#include <fruitlib/json/parse_projection.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/image/color/predef.hpp>
#include <sge/image/color/format.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/clear/parameters.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/context/scoped_ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/projection/perspective_af.hpp>
#include <sge/renderer/state/core/depth_stencil/object.hpp>
#include <sge/renderer/state/core/depth_stencil/parameters.hpp>
#include <sge/renderer/state/core/depth_stencil/scoped.hpp>
#include <sge/renderer/state/core/depth_stencil/depth/enabled.hpp>
#include <sge/renderer/state/core/depth_stencil/stencil/off.hpp>
#include <sge/renderer/target/from_texture.hpp>
#include <sge/renderer/target/offscreen.hpp>
#include <sge/renderer/texture/capabilities.hpp>
#include <sge/renderer/texture/capabilities_field.hpp>
#include <sge/renderer/texture/color_format.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/planar_parameters.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/text.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>


fruitapp::shadow_map::object::object(
	fruitlib::scenic::optional_parent const &_parent,
	sge::parse::json::object const &_config,
	sge::renderer::device::ffp &_renderer,
	fruitapp::light::manager const &_light_manager)
:
	node_base(
		_parent),
	renderer_(
		_renderer),
	depth_stencil_state_(
		_renderer.create_depth_stencil_state(
			sge::renderer::state::core::depth_stencil::parameters(
				sge::renderer::state::core::depth_stencil::depth::enabled(
					sge::renderer::state::core::depth_stencil::depth::func::less,
					sge::renderer::state::core::depth_stencil::depth::write_enable(
						true)),
				sge::renderer::state::core::depth_stencil::stencil::off()))),
	texture_(
		renderer_.create_planar_texture(
			sge::renderer::texture::planar_parameters(
				sge::parse::json::find_and_convert_member<sge::renderer::dim2>(
					_config,
					sge::parse::json::path(
						FCPPT_TEXT("size"))),
				sge::renderer::texture::color_format(
					sge::image::color::format::rgb32f,
					sge::renderer::texture::emulate_srgb::no),
				sge::renderer::texture::mipmap::off(),
				sge::renderer::resource_flags_field::null(),
				sge::renderer::texture::capabilities_field(
					sge::renderer::texture::capabilities::render_target)))),
	target_(
		sge::renderer::target::from_texture(
			renderer_,
			*texture_)),
	mvp_(
		fruitlib::perspective_projection_information_to_matrix(
			fruitlib::json::parse_projection(
				sge::parse::json::find_and_convert_member<sge::parse::json::object const>(
					_config,
					sge::parse::json::path(
						FCPPT_TEXT("projection"))),
				fcppt::optional<sge::renderer::scalar>())) *
		_light_manager.directional_source().model_view())
{
	// Do an initial clear of the texture to prevent race conditions
	// (the shadow map might be rendered before its first update)
	fruitlib::scenic::events::update event;

	this->react(
		event);
}

sge::renderer::texture::planar &
fruitapp::shadow_map::object::texture()
{
	return
		*texture_;
}

fruitapp::shadow_map::mvp const
fruitapp::shadow_map::object::mvp() const
{
	return
		mvp_;
}

fruitapp::shadow_map::object::~object()
{
}


void
fruitapp::shadow_map::object::react(
	fruitlib::scenic::events::update const &)
{
	sge::renderer::context::scoped_ffp scoped_context(
		renderer_,
		*target_);

	target_->clear(
		sge::renderer::clear::parameters()
			.back_buffer(
				sge::renderer::clear::back_buffer_value(
					sge::image::color::predef::black()))
			.depth_buffer(
				sge::renderer::clear::depth_buffer_value(
					1.0f)));

	sge::renderer::state::core::depth_stencil::scoped scoped_depth_stencil(
		scoped_context.get(),
		*depth_stencil_state_);

	fruitlib::scenic::events::render event(
		scoped_context.get());

	node_base::forward_to_children(
		event);
}
