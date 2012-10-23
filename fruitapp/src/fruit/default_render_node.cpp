#include <fruitapp/directional_light_source.hpp>
#include <fruitapp/media_path.hpp>
#include <fruitapp/fruit/default_render_node.hpp>
#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/fruit/model_vf/format.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/primitive_type.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_declaration.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/vector4.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/vertex_declaration.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/core.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/state/core/depth_stencil/object.hpp>
#include <sge/renderer/state/core/depth_stencil/parameters.hpp>
#include <sge/renderer/state/core/depth_stencil/scoped.hpp>
#include <sge/renderer/state/core/depth_stencil/depth/enabled.hpp>
#include <sge/renderer/state/core/depth_stencil/stencil/off.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/renderer/texture/scoped.hpp>
#include <sge/renderer/texture/stage.hpp>
#include <sge/shader/context.hpp>
#include <sge/shader/scoped_pair.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/inverse.hpp>
#include <fcppt/math/matrix/transpose.hpp>


fruitapp::fruit::default_render_node::default_render_node(
	fruitlib::scenic::optional_parent const &_parent,
	sge::shader::context &_shader_context,
	fruit::manager const &_manager,
	sge::camera::base const &_camera,
	fruitapp::directional_light_source const &light,
	fruitapp::ambient_intensity const &_ambient_intensity)
:
	node_base(
		_parent),
	manager_(
		_manager),
	camera_(
		_camera),
	shader_(
		_shader_context,
		_manager.vertex_declaration(),
		sge::shader::vertex_program_path(
			fruitapp::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model.cg")),
		sge::shader::pixel_program_path(
			fruitapp::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model.cg")),
		sge::shader::optional_cflags()),
	mvp_parameter_(
		shader_.vertex_program(),
		sge::shader::parameter::name(
			"mvp"),
		_shader_context.renderer(),
		sge::shader::parameter::is_projection_matrix(
			true),
		sge::renderer::matrix4()),
	mv_it_parameter_(
		shader_.vertex_program(),
		sge::shader::parameter::name(
			"mv_it"),
		_shader_context.renderer(),
		sge::shader::parameter::is_projection_matrix(
			false),
		sge::renderer::matrix4()),
	world_parameter_(
		shader_.vertex_program(),
		sge::shader::parameter::name(
			"world"),
		_shader_context.renderer(),
		sge::shader::parameter::is_projection_matrix(
			false),
		sge::renderer::matrix4()),
	light_position_parameter_(
		shader_.vertex_program(),
		sge::shader::parameter::name(
			"light_position"),
		light.position()),
	ambient_intensity_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"ambient_intensity"),
		_ambient_intensity.get()),
	diffuse_color_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"diffuse_color"),
		sge::renderer::vector4()),
	specular_color_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"specular_color"),
		sge::renderer::vector4()),
	diffuse_coefficient_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"diffuse_coefficient"),
		sge::renderer::scalar()),
	specular_coefficient_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"specular_coefficient"),
		sge::renderer::scalar()),
	specular_shininess_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"specular_shininess"),
		sge::renderer::scalar()),
	texture_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"input_texture"),
		shader_,
		_shader_context.renderer(),
		sge::shader::parameter::planar_texture::optional_value()),
	depth_stencil_state_(
		_shader_context.renderer().create_depth_stencil_state(
			sge::renderer::state::core::depth_stencil::parameters(
				sge::renderer::state::core::depth_stencil::depth::enabled(
					sge::renderer::state::core::depth_stencil::depth::func::less,
					sge::renderer::state::core::depth_stencil::depth::write_enable(
						true)),
				sge::renderer::state::core::depth_stencil::stencil::off())))
{
}

fruitapp::fruit::default_render_node::~default_render_node()
{
}

void
fruitapp::fruit::default_render_node::react(
	fruitlib::scenic::events::render const &_render_event)
{
	sge::renderer::state::core::depth_stencil::scoped scoped_depth_stencil(
		_render_event.context(),
		*depth_stencil_state_);

	sge::shader::scoped_pair scoped_shader(
		_render_event.context(),
		shader_);

	world_parameter_.set(
		sge::camera::matrix_conversion::world(
			camera_.coordinate_system()));

	sge::renderer::scoped_vertex_declaration scoped_vd(
		_render_event.context(),
		manager_.vertex_declaration());

	for(
		object_sequence::const_iterator i =
			manager_.fruits().begin();
		i != manager_.fruits().end();
		++i)
	{
		sge::renderer::scoped_vertex_buffer scoped_vb(
			_render_event.context(),
			i->vb());

		texture_parameter_.set(
			sge::shader::parameter::planar_texture::optional_value(
				*i->prototype().texture()));

		mvp_parameter_.set(
			sge::camera::matrix_conversion::world_projection(
				camera_.coordinate_system(),
				camera_.projection_matrix()) *
			i->world_transform());

		sge::renderer::matrix4 const world_transformation_matrix(
			sge::camera::matrix_conversion::world(
				camera_.coordinate_system()) *
			i->world_transform());

		mv_it_parameter_.set(
			fcppt::math::matrix::transpose(
				fcppt::math::matrix::inverse(
					world_transformation_matrix)));

		// Material shit
		diffuse_color_parameter_.set(
			i->prototype().material().diffuse_color());

		specular_color_parameter_.set(
			i->prototype().material().specular_color());

		diffuse_coefficient_parameter_.set(
			i->prototype().material().diffuse_coefficient());

		specular_coefficient_parameter_.set(
			i->prototype().material().specular_coefficient());

		specular_shininess_parameter_.set(
			i->prototype().material().specular_shininess());

		_render_event.context().render_nonindexed(
			sge::renderer::first_vertex(
				static_cast<sge::renderer::size_type>(
					0)),
			sge::renderer::vertex_count(
				i->vb().size()),
			sge::renderer::primitive_type::triangle_list);
	}
}
