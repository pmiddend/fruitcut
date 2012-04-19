#include <fruitapp/directional_light_source.hpp>
#include <fruitapp/fruit/default_render_node.hpp>
#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/fruit/model_vf/format.hpp>
#include <fruitlib/media_path.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/vector4.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/vertex_declaration.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/renderer/texture/scoped.hpp>
#include <sge/renderer/texture/stage.hpp>
#include <sge/renderer/texture/filter/scoped.hpp>
#include <sge/renderer/texture/filter/trilinear.hpp>
#include <sge/shader/activation_method.hpp>
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
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/inverse.hpp>
#include <fcppt/math/matrix/transpose.hpp>


fruitapp::fruit::default_render_node::default_render_node(
	fruitlib::scenic::optional_parent const &_parent,
	sge::renderer::device &_renderer,
	sge::renderer::vertex_declaration &_vertex_declaration,
	fruit::manager const &_manager,
	sge::camera::base const &_camera,
	fruitapp::directional_light_source const &light,
	sge::renderer::scalar const _ambient_intensity)
:
	node_base(
		_parent),
	renderer_(
		_renderer),
	manager_(
		_manager),
	camera_(
		_camera),
	shader_(
		sge::shader::object_parameters(
			renderer_,
			_vertex_declaration,
			sge::shader::vf_to_string<model_vf::format>(),
			fcppt::assign::make_container<sge::shader::variable_sequence>
				(sge::shader::variable(
					"mvp",
					sge::shader::variable_type::uniform,
					sge::shader::matrix(
						sge::renderer::matrix4(),
						sge::shader::matrix_flags::projection)))
				(sge::shader::variable(
					"mv",
					sge::shader::variable_type::uniform,
					sge::shader::matrix(
						sge::renderer::matrix4(),
						sge::shader::matrix_flags::none)))
				(sge::shader::variable(
					"normal_matrix",
					sge::shader::variable_type::uniform,
					sge::shader::matrix(
						sge::renderer::matrix4(),
						sge::shader::matrix_flags::none)))
				(sge::shader::variable(
					"world",
					sge::shader::variable_type::uniform,
					sge::shader::matrix(
						sge::renderer::matrix4(),
						sge::shader::matrix_flags::none)))
				(sge::shader::variable(
					"light_position",
					sge::shader::variable_type::constant,
					light.position()))
				(sge::shader::variable(
					"ambient_intensity",
					sge::shader::variable_type::constant,
					_ambient_intensity))
				(sge::shader::variable(
					"diffuse_color",
					sge::shader::variable_type::uniform,
					sge::renderer::vector4()))
				(sge::shader::variable(
					"specular_color",
					sge::shader::variable_type::uniform,
					sge::renderer::vector4()))
				(sge::shader::variable(
					"diffuse_coefficient",
					sge::shader::variable_type::uniform,
					sge::renderer::scalar()))
				(sge::shader::variable(
					"specular_coefficient",
					sge::shader::variable_type::uniform,
					sge::renderer::scalar()))
				(sge::shader::variable(
					"specular_shininess",
					sge::shader::variable_type::uniform,
					sge::renderer::scalar())),
			fcppt::assign::make_container<sge::shader::sampler_sequence>
				(sge::shader::sampler(
					"texture",
					sge::renderer::texture::planar_shared_ptr())))
				.name(
					FCPPT_TEXT("default fruit render node"))
				.vertex_shader(
					fruitlib::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model")/FCPPT_TEXT("vertex.glsl"))
				.fragment_shader(
					fruitlib::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model")/FCPPT_TEXT("fragment.glsl")))
{
}

fruitapp::fruit::default_render_node::~default_render_node()
{
}

void
fruitapp::fruit::default_render_node::react(
	fruitlib::scenic::events::render const &)
{
	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::depth_func::less));

	sge::renderer::texture::filter::scoped scoped_filter(
		renderer_,
		sge::renderer::texture::stage(
			0u),
		sge::renderer::texture::filter::trilinear());

	sge::shader::scoped scoped_shader(
		shader_,
		sge::shader::activation_method_field(
			sge::shader::activation_method::vertex_declaration));

	shader_.update_uniform(
		"world",
		sge::shader::matrix(
			sge::camera::matrix_conversion::world(
				camera_.coordinate_system()),
			sge::shader::matrix_flags::none));

	for(
		object_sequence::const_iterator i =
			manager_.fruits().begin();
		i != manager_.fruits().end();
		++i)
	{
		sge::renderer::scoped_vertex_buffer scoped_vb(
			renderer_,
			i->vb());

		sge::renderer::texture::scoped scoped_tex(
			renderer_,
			*i->prototype().texture(),
			sge::renderer::texture::stage(
				0u));

		shader_.update_uniform(
			"mvp",
			sge::shader::matrix(
				sge::camera::matrix_conversion::world_projection(
					camera_.coordinate_system(),
					camera_.projection_matrix()) *
				i->world_transform(),
				sge::shader::matrix_flags::projection));

		sge::renderer::matrix4 const world_transformation_matrix(
			sge::camera::matrix_conversion::world(
				camera_.coordinate_system()) *
			i->world_transform());

		shader_.update_uniform(
			"mv",
			sge::shader::matrix(
				world_transformation_matrix,
				sge::shader::matrix_flags::projection));

		shader_.update_uniform(
			"normal_matrix",
			sge::shader::matrix(
				fcppt::math::matrix::transpose(
					fcppt::math::matrix::inverse(
						world_transformation_matrix)),
				sge::shader::matrix_flags::projection));

		// Material shit
		shader_.update_uniform(
			"diffuse_color",
			i->prototype().material().diffuse_color());

		shader_.update_uniform(
			"specular_color",
			i->prototype().material().specular_color());

		shader_.update_uniform(
			"diffuse_coefficient",
			i->prototype().material().diffuse_coefficient());

		shader_.update_uniform(
			"specular_coefficient",
			i->prototype().material().specular_coefficient());

		shader_.update_uniform(
			"specular_shininess",
			i->prototype().material().specular_shininess());

		renderer_.render_nonindexed(
			sge::renderer::first_vertex(
				static_cast<sge::renderer::size_type>(
					0)),
			sge::renderer::vertex_count(
				i->vb().size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}
}
