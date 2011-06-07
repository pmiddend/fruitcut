#include "shadow_render_node.hpp"
#include "manager.hpp"
#include "model_vf/format.hpp"
#include "../../media_path.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_declaration.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/state/state.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/vertex_declaration.hpp>
#include <sge/shader/shader.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/matrix/matrix.hpp>

fruitcut::app::fruit::shadow_render_node::shadow_render_node(
	sge::renderer::device &_renderer,
	sge::renderer::vertex_declaration &_vertex_declaration,
	fruit::manager const &_manager,
	sge::renderer::matrix4 const &_mvp)
:
	renderer_(
		_renderer),
	vertex_declaration_(
		_vertex_declaration),
	manager_(
		_manager),
	shader_(
		sge::shader::object_parameters(
			renderer_,
			fruitcut::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("shadow_map_vertex.glsl"),
			fruitcut::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("shadow_map_fragment.glsl"),
			sge::shader::vf_to_string<fruit::model_vf::format>(),
			fcppt::assign::make_container<sge::shader::variable_sequence>(
				sge::shader::variable(
					"mvp",
					sge::shader::variable_type::uniform,
					sge::renderer::matrix4())),
			sge::shader::sampler_sequence())),
	mvp_(
		_mvp)
{
}

fruitcut::app::fruit::shadow_render_node::~shadow_render_node()
{
}

void
fruitcut::app::fruit::shadow_render_node::render()
{
	sge::shader::scoped scoped_shader(
		shader_,
		sge::shader::activation_method::with_textures);

	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::depth_func::less));

	sge::renderer::scoped_vertex_declaration scoped_decl(
		renderer_,
		vertex_declaration_);

	for(object_sequence::const_iterator i = manager_.fruits().begin(); i != manager_.fruits().end(); ++i)
	{
		sge::renderer::scoped_vertex_buffer scoped_vb(
			renderer_,
			i->vb());

		shader_.update_uniform(
			"mvp",
			mvp_ * i->world_transform());

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
fruitcut::app::fruit::shadow_render_node::update()
{
}
