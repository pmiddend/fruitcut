#include "default_render_node.hpp"
#include "../../media_path.hpp"
#include "model_vf/format.hpp"
#include "manager.hpp"
#include "../directional_light_source.hpp"
#include <sge/camera/object.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/stage_type.hpp>
#include <sge/renderer/state/state.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/scoped.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/shader/shader.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/text.hpp>

fruitcut::app::fruit::default_render_node::default_render_node(
	sge::renderer::device &_renderer,
	sge::renderer::vertex_declaration &_vertex_declaration,
	fruit::manager const &_manager,
	sge::camera::object &_camera,
	app::directional_light_source const &light,
	sge::renderer::scalar const _ambient_intensity)
:
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
			fruitcut::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model")/FCPPT_TEXT("vertex.glsl"),
			fruitcut::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model")/FCPPT_TEXT("fragment.glsl"),
			sge::shader::vf_to_string<model_vf::format>(),
			fcppt::assign::make_container<sge::shader::variable_sequence>
				(sge::shader::variable(
					"mvp",
					sge::shader::variable_type::uniform,
					sge::renderer::matrix4()))
				(sge::shader::variable(
					"mv",
					sge::shader::variable_type::uniform,
					sge::renderer::matrix4()))
				(sge::shader::variable(
					"normal_matrix",
					sge::shader::variable_type::uniform,
					sge::renderer::matrix4()))
				(sge::shader::variable(
					"world",
					sge::shader::variable_type::uniform,
					sge::renderer::matrix4()))
				(sge::shader::variable(
					"light_position",
					sge::shader::variable_type::const_,
					light.position()))
				(sge::shader::variable(
					"ambient_intensity",
					sge::shader::variable_type::const_,
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
					sge::renderer::texture::planar_ptr()))))
{
}

fruitcut::app::fruit::default_render_node::~default_render_node()
{
}

void
fruitcut::app::fruit::default_render_node::render()
{
	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::depth_func::less));

	sge::shader::scoped scoped_shader(
		shader_,
		sge::shader::activation_method::vertex_declaration);

	shader_.update_uniform(
		"world",
		camera_.world());

	for(object_sequence::const_iterator i = manager_.fruits().begin(); i != manager_.fruits().end(); ++i)
	{
		sge::renderer::scoped_vertex_buffer scoped_vb(
			renderer_,
			i->vb());

		sge::renderer::texture::scoped scoped_tex(
			renderer_,
			*i->prototype().texture(),
			static_cast<sge::renderer::stage_type>(
				0));

		shader_.update_uniform(
			"mvp",
			camera_.mvp() * i->world_transform());

		shader_.update_uniform(
			"mv",
			camera_.world() * i->world_transform());

		shader_.update_uniform(
			"normal_matrix",
			fcppt::math::matrix::transpose(
				fcppt::math::matrix::inverse(
					camera_.world() * i->world_transform())));

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

void
fruitcut::app::fruit::default_render_node::update()
{
}