#include "mesh_to_vertex_buffer.hpp"
#include "mesh.hpp"
#include "triangle.hpp"
#include "model_vf/format.hpp"
#include "model_vf/vertex_view.hpp"
#include "model_vf/position.hpp"
#include "model_vf/texcoord.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/vf/dynamic/part_index.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/shader/object.hpp>
#include <boost/foreach.hpp>

sge::renderer::vertex_buffer_ptr const
fruitcut::app::fruit::mesh_to_vertex_buffer(
	sge::renderer::device &renderer,
	sge::renderer::vertex_declaration &vertex_decl,
	sge::shader::object &model_shader,
	mesh const &m)
{
	sge::renderer::vertex_buffer_ptr const vb = 
		renderer.create_vertex_buffer(
			vertex_decl,
			sge::renderer::vf::dynamic::part_index(
				0u),
			static_cast<sge::renderer::size_type>(
				m.triangles.size() * 3),
			sge::renderer::resource_flags::none);

	// We don't need to scope the "whole" shader with the texture
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer,
		model_shader.program());

	sge::renderer::scoped_vertex_lock const vblock(
		*vb,
		sge::renderer::lock_mode::writeonly);

	model_vf::vertex_view vertices(
		vblock.value());

	model_vf::vertex_view::iterator vb_it(
		vertices.begin());

	BOOST_FOREACH(
		triangle const &t,
		m.triangles)
	{
		(vb_it)->set<model_vf::position>(
			t.vertices[0]);
		(vb_it++)->set<model_vf::texcoord>(
			t.texcoords[0]);
		(vb_it)->set<model_vf::position>(
			t.vertices[1]);
		(vb_it++)->set<model_vf::texcoord>(
			t.texcoords[1]);
		(vb_it)->set<model_vf::position>(
			t.vertices[2]);
		(vb_it++)->set<model_vf::texcoord>(
			t.texcoords[2]);
	}

	return vb;
}
