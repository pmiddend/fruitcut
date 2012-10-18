#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/fruit/shadow_render_node.hpp>
#include <fruitapp/fruit/model_vf/format.hpp>
#include <fruitlib/media_path.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/vertex_declaration.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/core.hpp>
#include <sge/renderer/state/core/depth_stencil/object.hpp>
#include <sge/renderer/state/core/depth_stencil/parameters.hpp>
#include <sge/renderer/state/core/depth_stencil/scoped.hpp>
#include <sge/renderer/state/core/depth_stencil/depth/enabled.hpp>
#include <sge/renderer/state/core/depth_stencil/stencil/off.hpp>
#include <sge/shader/context.hpp>
#include <sge/shader/scoped_pair.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/object_impl.hpp>


fruitapp::fruit::shadow_render_node::shadow_render_node(
	fruitlib::scenic::optional_parent const &_parent,
	sge::shader::context &_shader_context,
	sge::renderer::vertex_declaration &_vertex_declaration,
	fruitapp::fruit::manager const &_manager,
	fruitapp::shadow_mvp const &_mvp)
:
	node_base(
		_parent),
	vertex_declaration_(
		_vertex_declaration),
	manager_(
		_manager),
	shader_(
		_shader_context,
		_vertex_declaration,
		sge::shader::vertex_program_path(
			fruitlib::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model_shadow.cg")),
		sge::shader::pixel_program_path(
			fruitlib::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model_shadow.cg")),
		sge::shader::optional_cflags()),
	mvp_parameter_(
		shader_.vertex_program(),
		sge::shader::parameter::name(
			"mvp"),
		_shader_context.renderer(),
		sge::shader::parameter::is_projection_matrix(
			true),
		sge::renderer::matrix4()),
	mvp_(
		_mvp),
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

fruitapp::fruit::shadow_render_node::~shadow_render_node()
{
}

void
fruitapp::fruit::shadow_render_node::react(
	fruitlib::scenic::events::render const &_render_event)
{
	sge::renderer::state::core::depth_stencil::scoped scoped_depth_stencil(
		_render_event.context(),
		*depth_stencil_state_);

	sge::shader::scoped_pair scoped_shader(
		_render_event.context(),
		shader_);

	for(object_sequence::const_iterator i = manager_.fruits().begin(); i != manager_.fruits().end(); ++i)
	{
		sge::renderer::scoped_vertex_buffer scoped_vb(
			_render_event.context(),
			i->vb());

		mvp_parameter_.set(
			mvp_.get() * i->world_transform());

		_render_event.context().render_nonindexed(
			sge::renderer::first_vertex(
				static_cast<sge::renderer::size_type>(
					0)),
			sge::renderer::vertex_count(
				i->vb().size()),
			sge::renderer::primitive_type::triangle_list);
	}
}
