#include "create_quad.hpp"
#include "vertex_view.hpp"
#include "format.hpp"
#include <sge/shader/scoped.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/vertex_declaration_ptr.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/vf/dynamic/part_index.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

fruitcut::pp::screen_vf::declaration_buffer_pair const
fruitcut::pp::screen_vf::create_quad(
	sge::shader::object &shader,
	sge::renderer::device_ptr const renderer)
{
	sge::renderer::vertex_declaration_ptr const vertex_declaration(
		renderer->create_vertex_declaration(
			sge::renderer::vf::dynamic::make_format<format>()));

	sge::renderer::vertex_buffer_ptr const vb(
		renderer->create_vertex_buffer(
			vertex_declaration,
			sge::renderer::vf::dynamic::part_index(
				0u),
			6,
			sge::renderer::resource_flags::none));
	
	sge::shader::scoped scoped_shader(
		shader);

	sge::renderer::scoped_vertex_lock const vblock(
		vb,
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

	return 
		declaration_buffer_pair(
			vertex_declaration,
			vb);
}
