#include "quad.hpp"
#include "vertex_view.hpp"
#include "format.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/vertex_declaration_ptr.hpp>
#include <sge/renderer/vf/vf.hpp>
#include <sge/shader/shader.hpp>
#include <fcppt/math/vector/vector.hpp>

fruitcut::fruitlib::pp::screen_vf::quad::quad(
	sge::renderer::device &_renderer)
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
fruitcut::fruitlib::pp::screen_vf::quad::render()
{
	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		*buffer_);

	renderer_.render_nonindexed(
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			buffer_->size()),
		sge::renderer::nonindexed_primitive_type::triangle);
}

sge::renderer::vertex_declaration &
fruitcut::fruitlib::pp::screen_vf::quad::vertex_declaration()
{
	return *declaration_;
}

sge::renderer::vertex_declaration const &
fruitcut::fruitlib::pp::screen_vf::quad::vertex_declaration() const
{
	return *declaration_;
}

fruitcut::fruitlib::pp::screen_vf::quad::~quad()
{
}