#include "create_quad.hpp"
#include "vertex_view.hpp"
#include <sge/shader/scoped.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

sge::renderer::vertex_buffer_ptr const
fruitcut::sandbox::screen_vf::create_quad(
	sge::shader::object &shader,
	sge::renderer::device_ptr const renderer)
{
	sge::renderer::vertex_buffer_ptr const vb(
		renderer->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<format>(),
			static_cast<sge::renderer::size_type>(
				6),
			sge::renderer::resource_flags::none));
	
	sge::shader::scoped scoped_shader(
		shader);
	
	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer,
		vb);

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

	return vb;
}
