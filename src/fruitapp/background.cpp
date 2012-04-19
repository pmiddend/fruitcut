#include <fruitapp/background.hpp>
#include <fruitlib/media_path.hpp>
#include <fruitlib/math/view_plane_rect.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/viewport_change.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/matrix_mode.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_declaration.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/target_base.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/vertex_declaration.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/viewport_size.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/draw_mode.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/stencil_func.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/texture/create_planar_from_path.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <sge/renderer/vf/format.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/vf/part.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vector.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/vf/dynamic/part_index.hpp>
#include <sge/shader/activate_everything.hpp>
#include <sge/shader/matrix.hpp>
#include <sge/shader/matrix_flags.hpp>
#include <sge/shader/object_parameters.hpp>
#include <sge/shader/sampler.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/variant/holds_type.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


namespace
{
namespace vf
{
namespace tags
{
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(position);
}

typedef
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		sge::renderer::scalar,
		2
	>,
	tags::position
>
position;

namespace tags
{
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(texcoord);
}

typedef
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		sge::renderer::scalar,
		2
	>,
	tags::texcoord
>
texcoord;

typedef
sge::renderer::vf::part
<
	boost::mpl::vector2
	<
		position,
		texcoord
	>
>
part;

typedef
sge::renderer::vf::format
<
	boost::mpl::vector1
	<
		part
	>
>
format;

typedef
sge::renderer::vf::view<part>
vertex_view;
}
}

fruitapp::background::background(
	fruitlib::scenic::optional_parent const &_parent,
	sge::renderer::device &_renderer,
	sge::image2d::system &_image_loader,
	fruitapp::shadow_mvp const &_shadow_mvp,
	fruitapp::shadow_map_texture const &_shadow_map_texture,
	sge::parse::json::object const &_config,
	sge::camera::base const &_camera)
:
	node_base(
		_parent),
	renderer_(
		_renderer),
	camera_(
		_camera),
	texture_(
		sge::renderer::texture::create_planar_from_path(
			fruitlib::media_path()
				/
					FCPPT_TEXT("textures")
				/
					sge::parse::json::find_and_convert_member<fcppt::string>(
						_config,
						sge::parse::json::path(
							FCPPT_TEXT("textures"))/
							FCPPT_TEXT("background")),
			renderer_,
			_image_loader,
			sge::renderer::texture::mipmap::off(),
			sge::renderer::resource_flags::none)),
	vertex_declaration_(
		renderer_.create_vertex_declaration(
			sge::renderer::vf::dynamic::make_format<vf::format>())),
	vb_(
		renderer_.create_vertex_buffer(
			*vertex_declaration_,
			sge::renderer::vf::dynamic::part_index(
				0u),
			sge::renderer::vertex_count(6u),
			sge::renderer::resource_flags::none)),
	shader_(
		sge::shader::object_parameters(
			renderer_,
			*vertex_declaration_,
			sge::shader::vf_to_string<vf::format>(),
			fcppt::assign::make_container<sge::shader::variable_sequence>
				(sge::shader::variable(
					"mvp",
					sge::shader::variable_type::uniform,
					sge::shader::matrix(
						sge::renderer::matrix4(),
						sge::shader::matrix_flags::projection)))
				(sge::shader::variable(
					"shadow_mvp",
					sge::shader::variable_type::uniform,
					sge::shader::matrix(
						_shadow_mvp.get(),
						sge::shader::matrix_flags::projection))),
			fcppt::assign::make_container<sge::shader::sampler_sequence>
				(sge::shader::sampler(
					"tex",
					texture_))
				(sge::shader::sampler(
					"shadow_map",
					_shadow_map_texture.get())))
				.name(
					FCPPT_TEXT("background"))
				.vertex_shader(
					fruitlib::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("background")/FCPPT_TEXT("vertex.glsl"))
				.fragment_shader(
					fruitlib::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("background")/FCPPT_TEXT("fragment.glsl"))),
	reps_(
		sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
			_config,
			sge::parse::json::path(
				FCPPT_TEXT("background-repeat"))))
{
}

fruitapp::background::~background()
{
}

void
fruitapp::background::react(
	fruitlib::scenic::events::render const &)
{
	sge::shader::scoped scoped_shader(
		shader_,
		sge::shader::activate_everything());

	sge::renderer::scoped_vertex_buffer scoped_vb(
		renderer_,
		*vb_);

	shader_.update_uniform(
		"mvp",
		sge::shader::matrix(
			sge::camera::matrix_conversion::world_projection(
				camera_.coordinate_system(),
				camera_.projection_matrix()),
			sge::shader::matrix_flags::projection));

	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::bool_::enable_alpha_blending = false)
			(sge::renderer::state::cull_mode::off)
			(sge::renderer::state::depth_func::less)
			(sge::renderer::state::stencil_func::off)
			(sge::renderer::state::draw_mode::fill));

	renderer_.render_nonindexed(
		sge::renderer::first_vertex(0u),
		sge::renderer::vertex_count(
			6u),
		sge::renderer::nonindexed_primitive_type::triangle);
}

void
fruitapp::background::react(
	fruitapp::projection_manager::projection_change const &_projection_change)
{
	typedef
	fcppt::math::box::object<sge::renderer::scalar,2>
	scalar_rect;

	// zero plane because it's the visible plane located at z = 0
	scalar_rect const zero_plane(
		fruitlib::math::view_plane_rect(
			sge::camera::matrix_conversion::world_projection(
				camera_.coordinate_system(),
				camera_.projection_matrix()),
			_projection_change.perspective_projection_information()));

	sge::renderer::scoped_vertex_lock const vblock(
		*vb_,
		sge::renderer::lock_mode::writeonly);

	vf::vertex_view const vertices(
		vblock.value());

	vf::vertex_view::iterator vb_it(
		vertices.begin());

	// Left top
	(vb_it)->set<vf::position>(
		vf::position::packed_type(
			zero_plane.left(), zero_plane.top()));
	(vb_it++)->set<vf::texcoord>(
		vf::texcoord::packed_type(
			0,0));

	// Left bottom
	(vb_it)->set<vf::position>(
		vf::position::packed_type(
			zero_plane.left(),zero_plane.bottom()));
	(vb_it++)->set<vf::texcoord>(
		vf::texcoord::packed_type(
			0,reps_));

	// Right top
	(vb_it)->set<vf::position>(
		vf::position::packed_type(
			zero_plane.right(),zero_plane.top()));
	(vb_it++)->set<vf::texcoord>(
		vf::texcoord::packed_type(
			reps_,0));

	// Right top
	(vb_it)->set<vf::position>(
		vf::position::packed_type(
			zero_plane.right(),zero_plane.top()));
	(vb_it++)->set<vf::texcoord>(
		vf::texcoord::packed_type(
			reps_,0));

	// Left bottom
	(vb_it)->set<vf::position>(
		vf::position::packed_type(
			zero_plane.left(),zero_plane.bottom()));
	(vb_it++)->set<vf::texcoord>(
		vf::texcoord::packed_type(
			0,reps_));

	// Right bottom
	(vb_it)->set<vf::position>(
		vf::position::packed_type(
			zero_plane.right(),zero_plane.bottom()));
	(vb_it++)->set<vf::texcoord>(
		vf::texcoord::packed_type(
			reps_,reps_));
}
