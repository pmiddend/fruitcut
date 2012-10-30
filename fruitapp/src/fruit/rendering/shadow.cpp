#include <fruitapp/media_path.hpp>
#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/fruit/rendering/shadow.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_declaration.hpp>
#include <sge/renderer/vertex_buffer.hpp>
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
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/object_impl.hpp>


fruitapp::fruit::rendering::shadow::shadow(
	sge::shader::context &_shader_context,
	fruitapp::fruit::manager const &_manager,
	fruitapp::shadow_map::mvp const &_mvp)
:
	manager_(
		_manager),
	shader_(
		_shader_context,
		_manager.vertex_declaration(),
		sge::shader::vertex_program_path(
			fruitapp::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model_shadow.cg")),
		sge::shader::pixel_program_path(
			fruitapp::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model_shadow.cg")),
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

fruitapp::fruit::rendering::shadow::~shadow()
{
}

void
fruitapp::fruit::rendering::shadow::render(
	sge::renderer::context::core &_context)
{
	sge::renderer::state::core::depth_stencil::scoped scoped_depth_stencil(
		_context,
		*depth_stencil_state_);

	sge::renderer::scoped_vertex_declaration scoped_vd(
		_context,
		manager_.vertex_declaration());

	sge::shader::scoped_pair scoped_shader(
		_context,
		shader_);

	for(
		fruitapp::fruit::object_sequence::const_iterator i =
			manager_.fruits().begin();
		i != manager_.fruits().end();
		++i)
	{
		sge::renderer::scoped_vertex_buffer scoped_vb(
			_context,
			i->vb());

		mvp_parameter_.set(
			mvp_.get() * i->world_transform());

		_context.render_nonindexed(
			sge::renderer::first_vertex(
				static_cast<sge::renderer::size_type>(
					0)),
			sge::renderer::vertex_count(
				i->vb().size()),
			sge::renderer::primitive_type::triangle_list);
	}
}
