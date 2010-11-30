#include "mesh_to_vertex_buffer.hpp"
#include "mesh.hpp"
#include "triangle.hpp"
#include "model/vf/format.hpp"
#include "model/vf/position.hpp"
#include "model/vf/texcoord.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/shader/object.hpp>
#include <boost/foreach.hpp>

namespace
{
typedef 
sge::renderer::vf::view
<
	fruitcut::model::vf::format
> 
vertex_view;
}

sge::renderer::vertex_buffer_ptr const
fruitcut::mesh_to_vertex_buffer(
	sge::renderer::device_ptr const renderer,
	sge::shader::object &model_shader,
	mesh const &m)
{
	sge::renderer::vertex_buffer_ptr const vb = 
		renderer->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<model::vf::format>(),
			static_cast<sge::renderer::size_type>(
				m.triangles.size() * 3),
			sge::renderer::resource_flags::none);

	// We don't need to scope the "whole" shader with the texture
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer,
		model_shader.program());

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

	BOOST_FOREACH(
		triangle const &t,
		m.triangles)
	{
		(vb_it)->set<model::vf::position>(
			t.vertices[0]);
		(vb_it++)->set<model::vf::texcoord>(
			t.texcoords[0]);
		(vb_it)->set<model::vf::position>(
			t.vertices[1]);
		(vb_it++)->set<model::vf::texcoord>(
			t.texcoords[1]);
		(vb_it)->set<model::vf::position>(
			t.vertices[2]);
		(vb_it++)->set<model::vf::texcoord>(
			t.texcoords[2]);
	}

	return vb;
}
