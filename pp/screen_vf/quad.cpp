#include "quad.hpp"
#include "vertex_view.hpp"
#include "format.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_declaration.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/vertex_declaration_ptr.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/vf/dynamic/part_index.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/shader/scoped.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/assert.hpp>

fruitcut::pp::screen_vf::quad::quad(
	sge::renderer::device &_renderer,
	sge::shader::object &_shader)
:
	renderer_(
		_renderer),
	declaration_(
		renderer_.create_vertex_declaration(
			sge::renderer::vf::dynamic::make_format<format>())),
	buffer_(
		renderer_.create_vertex_buffer(
			*declaration_,
			sge::renderer::vf::dynamic::part_index(
				0u),
			6,
			sge::renderer::resource_flags::none))
{
	sge::shader::scoped scoped_shader(
		_shader);

	sge::renderer::scoped_vertex_lock const vblock(
		*buffer_,
		sge::renderer::lock_mode::writeonly);

	vertex_view const vertices(
		vblock.value());

	vertex_view::iterator vb_it(
		vertices.begin());

	// Left top
	(vb_it++)->set<position>(
		position::packed_type(
			-1, 1));

	// Left bottom
	(vb_it++)->set<position>(
		position::packed_type(
			-1,-1));

	// Right top
	(vb_it++)->set<position>(
		position::packed_type(
			1,1));

	// Right top
	(vb_it++)->set<position>(
		position::packed_type(
			1,1));

	// Left bottom
	(vb_it++)->set<position>(
		position::packed_type(
			-1,-1));

	// Right bottom
	(vb_it++)->set<position>(
		position::packed_type(
			1,-1));
}

void
fruitcut::pp::screen_vf::quad::render()
{
	FCPPT_ASSERT(
		declaration_ && buffer_);

	sge::renderer::scoped_vertex_declaration const vb_declaration_context(
		renderer_,
		*declaration_);

	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		*buffer_);

	renderer_.render(
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			buffer_->size()),
		sge::renderer::nonindexed_primitive_type::triangle);
}

fruitcut::pp::screen_vf::quad::~quad()
{
}
