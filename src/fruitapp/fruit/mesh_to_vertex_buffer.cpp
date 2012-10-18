#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/mesh_to_vertex_buffer.hpp>
#include <fruitapp/fruit/triangle.hpp>
#include <fruitapp/fruit/model_vf/format.hpp>
#include <fruitapp/fruit/model_vf/normal.hpp>
#include <fruitapp/fruit/model_vf/position.hpp>
#include <fruitapp/fruit/model_vf/texcoord.hpp>
#include <fruitapp/fruit/model_vf/vertex_view.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/resource_flags.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_buffer_unique_ptr.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>
#include <sge/renderer/device/core.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/dynamic/part_index.hpp>
#include <fcppt/move.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>


sge::renderer::vertex_buffer_unique_ptr
fruitapp::fruit::mesh_to_vertex_buffer(
	sge::renderer::device::core &renderer,
	sge::renderer::vertex_declaration &vertex_decl,
	mesh const &m)
{
	sge::renderer::vertex_buffer_unique_ptr vb(
		renderer.create_vertex_buffer(
			vertex_decl,
			sge::renderer::vf::dynamic::part_index(
				0u),
			sge::renderer::vertex_count(
				static_cast<sge::renderer::size_type>(
					m.triangles().size() * 3)),
			sge::renderer::resource_flags_field::null()));

	sge::renderer::scoped_vertex_lock const vblock(
		*vb,
		sge::renderer::lock_mode::writeonly);

	fruitapp::fruit::model_vf::vertex_view vertices(
		vblock.value());

	fruitapp::fruit::model_vf::vertex_view::iterator vb_it(
		vertices.begin());

	for(
		fruit::mesh::triangle_sequence::const_iterator t =
			m.triangles().begin();
		t != m.triangles().end();
		++t)
	{
		(vb_it)->set<fruitapp::fruit::model_vf::position>(
			t->vertices[0]);
		(vb_it)->set<fruitapp::fruit::model_vf::normal>(
			t->normals[0]);
		(vb_it++)->set<fruitapp::fruit::model_vf::texcoord>(
			t->texcoords[0]);
		(vb_it)->set<fruitapp::fruit::model_vf::position>(
			t->vertices[1]);
		(vb_it)->set<fruitapp::fruit::model_vf::normal>(
			t->normals[1]);
		(vb_it++)->set<fruitapp::fruit::model_vf::texcoord>(
			t->texcoords[1]);
		(vb_it)->set<fruitapp::fruit::model_vf::position>(
			t->vertices[2]);
		(vb_it)->set<fruitapp::fruit::model_vf::normal>(
			t->normals[2]);
		(vb_it++)->set<fruitapp::fruit::model_vf::texcoord>(
			t->texcoords[2]);
	}

	return
		fcppt::move(
			vb);
}
