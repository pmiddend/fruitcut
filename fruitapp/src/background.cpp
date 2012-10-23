#include <fruitapp/background.hpp>
#include <fruitapp/media_path.hpp>
#include <fruitapp/projection_manager/object.hpp>
#include <fruitlib/math/view_plane_rect.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/pixel_rect.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_declaration.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_declaration.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/core.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/state/core/depth_stencil/object.hpp>
#include <sge/renderer/state/core/depth_stencil/parameters.hpp>
#include <sge/renderer/state/core/depth_stencil/scoped.hpp>
#include <sge/renderer/texture/create_planar_from_path.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <sge/renderer/vf/format.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/part.hpp>
#include <sge/renderer/vf/pos.hpp>
#include <sge/renderer/vf/texpos.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/vf/dynamic/part_index.hpp>
#include <sge/shader/context.hpp>
#include <sge/shader/scoped_pair.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>
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
typedef
sge::renderer::vf::pos
<
	sge::renderer::scalar,
	2
>
position;

typedef
sge::renderer::vf::texpos
<
	sge::renderer::scalar,
	2,
	sge::renderer::vf::index<0u>
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
	sge::image2d::system &_image_loader,
	sge::shader::context &_shader_context,
	fruitapp::shadow_mvp const &_shadow_mvp,
	fruitapp::shadow_map_texture const &_shadow_map_texture,
	sge::parse::json::object const &_config,
	sge::camera::base const &_camera,
	fruitapp::projection_manager::object &_projection_manager)
:
	node_base(
		_parent),
	camera_(
		_camera),
	texture_(
		sge::renderer::texture::create_planar_from_path(
			fruitapp::media_path()
				/
					FCPPT_TEXT("textures")
				/
					sge::parse::json::find_and_convert_member<fcppt::string>(
						_config,
						sge::parse::json::path(
							FCPPT_TEXT("textures"))/
							FCPPT_TEXT("background")),
			_shader_context.renderer(),
			_image_loader,
			sge::renderer::texture::mipmap::off(),
			sge::renderer::resource_flags_field::null())),
	vertex_declaration_(
		_shader_context.renderer().create_vertex_declaration(
			sge::renderer::vf::dynamic::make_format<vf::format>())),
	vb_(
		_shader_context.renderer().create_vertex_buffer(
			*vertex_declaration_,
			sge::renderer::vf::dynamic::part_index(
				0u),
			sge::renderer::vertex_count(
				6u),
			sge::renderer::resource_flags_field::null())),
	shader_(
		_shader_context,
		*vertex_declaration_,
		sge::shader::vertex_program_path(
			fruitapp::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("background.cg")),
		sge::shader::pixel_program_path(
			fruitapp::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("background.cg")),
		sge::shader::optional_cflags()),
	mvp_parameter_(
		shader_.vertex_program(),
		sge::shader::parameter::name(
			"mvp"),
		_shader_context.renderer(),
		sge::shader::parameter::is_projection_matrix(
			true),
		sge::renderer::matrix4()),
	shadow_mvp_parameter_(
		shader_.vertex_program(),
		sge::shader::parameter::name(
			"shadow_mvp"),
		_shader_context.renderer(),
		sge::shader::parameter::is_projection_matrix(
			true),
		_shadow_mvp.get()),
	texture_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"input_texture"),
		shader_,
		_shader_context.renderer(),
		sge::shader::parameter::planar_texture::optional_value(
			*texture_)),
	shadow_map_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"shadow_texture"),
		shader_,
		_shader_context.renderer(),
		sge::shader::parameter::planar_texture::optional_value(
			_shadow_map_texture.get())),
	depth_stencil_state_(
		_shader_context.renderer().create_depth_stencil_state(
			sge::renderer::state::core::depth_stencil::parameters(
				sge::renderer::state::core::depth_stencil::depth::enabled(
					sge::renderer::state::core::depth_stencil::depth::func::less,
					sge::renderer::state::core::depth_stencil::depth::write_enable(
						true)),
				sge::renderer::state::core::depth_stencil::stencil::off()))),
	reps_(
		sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
			_config,
			sge::parse::json::path(
				FCPPT_TEXT("background-repeat")))),
	projection_change_connection_(
		_projection_manager.projection_change_callback(
			std::tr1::bind(
				&background::projection_change,
				this,
				std::tr1::placeholders::_1),
			fruitapp::projection_manager::trigger_early(
				true)))
{
}

fruitapp::background::~background()
{
}

void
fruitapp::background::react(
	fruitlib::scenic::events::render const &_render_event)
{
	sge::renderer::scoped_vertex_declaration scoped_vertex_declaration(
		_render_event.context(),
		*vertex_declaration_);

	sge::renderer::scoped_vertex_buffer scoped_vb(
		_render_event.context(),
		*vb_);

	sge::shader::scoped_pair scoped_shader(
		_render_event.context(),
		shader_);

	mvp_parameter_.set(
		sge::camera::matrix_conversion::world_projection(
			camera_.coordinate_system(),
			camera_.projection_matrix()));

	sge::renderer::state::core::depth_stencil::scoped scoped_depth_stencil(
		_render_event.context(),
		*depth_stencil_state_);

	_render_event.context().render_nonindexed(
		sge::renderer::first_vertex(
			0u),
		sge::renderer::vertex_count(
			6u),
		sge::renderer::primitive_type::triangle_list);
}

void
fruitapp::background::projection_change(
	fruitlib::perspective_projection_information const &_perspective_projection_information)
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
			_perspective_projection_information));

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
