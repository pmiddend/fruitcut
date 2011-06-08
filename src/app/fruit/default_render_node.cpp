#include "default_render_node.hpp"
#include "../../media_path.hpp"
#include "model_vf/format.hpp"
#include "manager.hpp"
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
	sge::camera::object &_camera)
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
					sge::renderer::matrix4())),
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
