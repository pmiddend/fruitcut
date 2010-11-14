#include "running.hpp"
#include "../media_path.hpp"
#include "../model/vf/format.hpp"
#include "../json/find_member.hpp"
#include "../model_to_mesh.hpp"
#include "../mesh_to_vertex_buffer.hpp"
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/shader/mat4.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/draw_mode.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/model/loader.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>

fruitcut::states::running::running(
	my_context const ctx)
:
	my_base(
		ctx),
	shader_(
		context<machine>().systems().renderer(),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("fragment.glsl"),
		sge::shader::vf_to_string<model::vf::format>(),
		{
			sge::shader::variable(
				"mvp",
				sge::shader::variable_type::uniform,
				sge::shader::mat4())
		},
		{}),
	model_(
		context<machine>().systems().md3_loader()->load(
			media_path()/
			FCPPT_TEXT("models")/
			json::find_member<fcppt::string>(
				context<machine>().config_file(),
				FCPPT_TEXT("test-model")))),
	mesh_(
		model_to_mesh(
			model_)),
	mesh_vb_(
		mesh_to_vertex_buffer(
			context<machine>().systems().renderer(),
			shader_,
			mesh_)),
	scoped_state_(
		context<machine>().systems().renderer(),
		sge::renderer::state::list
			(sge::renderer::state::bool_::clear_backbuffer = true))
{
}

boost::statechart::result
fruitcut::states::running::react(
	events::tick const &t)
{
	context<machine>().camera().update(
		t.time_delta());
	return discard_event();
}

boost::statechart::result
fruitcut::states::running::react(
	events::render const &)
{
	sge::shader::scoped scoped_shader(
		shader_);

	context<machine>().systems().renderer()->state(
		sge::renderer::state::list
			(sge::renderer::state::draw_mode::line));

	shader_.set_uniform(
		"mvp",
		context<machine>().camera().mvp());

	sge::renderer::scoped_vertex_buffer scoped_vb(
		context<machine>().systems().renderer(),
		mesh_vb_);

	context<machine>().systems().renderer()->render(
		sge::renderer::first_vertex(0),
		sge::renderer::vertex_count(
			mesh_vb_->size()),
		sge::renderer::nonindexed_primitive_type::triangle);

	return discard_event();
}
