#include <fruitapp/fruit/shadow_render_node.hpp>
#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/fruit/model_vf/format.hpp>
#include "../../media_path.hpp"
#include <fruitlib/scenic/events/render.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/state/state.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/vertex_declaration.hpp>
#include <sge/shader/object_parameters.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/shader/matrix.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/shader/matrix_flags.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/activate_everything.hpp>
#include <sge/shader/scoped.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/matrix/matrix.hpp>

fruitapp::fruit::shadow_render_node::shadow_render_node(
	fruitlib::scenic::optional_parent const &_parent,
	sge::renderer::device &_renderer,
	sge::renderer::vertex_declaration &_vertex_declaration,
	fruit::manager const &_manager,
	sge::renderer::matrix4 const &_mvp)
:
	node_base(
		_parent),
	renderer_(
		_renderer),
	vertex_declaration_(
		_vertex_declaration),
	manager_(
		_manager),
	shader_(
		sge::shader::object_parameters(
			renderer_,
			vertex_declaration_,
			fruitcut::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("shadow_map")/FCPPT_TEXT("vertex.glsl"),
			fruitcut::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("shadow_map")/FCPPT_TEXT("fragment.glsl"),
			sge::shader::vf_to_string<fruit::model_vf::format>(),
			fcppt::assign::make_container<sge::shader::variable_sequence>(
				sge::shader::variable(
					"mvp",
					sge::shader::variable_type::uniform,
					sge::shader::matrix(
						sge::renderer::matrix4(),
						sge::shader::matrix_flags::projection))),
			sge::shader::sampler_sequence())),
	mvp_(
		_mvp)
{
}

fruitapp::fruit::shadow_render_node::~shadow_render_node()
{
}

void
fruitapp::fruit::shadow_render_node::react(
	fruitlib::scenic::events::render const &)
{
	sge::shader::scoped scoped_shader(
		shader_,
		sge::shader::activate_everything());

	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::depth_func::less));

	for(object_sequence::const_iterator i = manager_.fruits().begin(); i != manager_.fruits().end(); ++i)
	{
		sge::renderer::scoped_vertex_buffer scoped_vb(
			renderer_,
			i->vb());

		shader_.update_uniform(
			"mvp",
			sge::shader::matrix(
				mvp_ * i->world_transform(),
				sge::shader::matrix_flags::projection));

		renderer_.render_nonindexed(
			sge::renderer::first_vertex(
				static_cast<sge::renderer::size_type>(
					0)),
			sge::renderer::vertex_count(
				i->vb().size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}
}
