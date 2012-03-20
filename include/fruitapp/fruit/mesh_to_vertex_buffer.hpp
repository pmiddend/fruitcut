#ifndef FRUITAPP_FRUIT_MESH_TO_VERTEX_BUFFER_HPP_INCLUDED
#define FRUITAPP_FRUIT_MESH_TO_VERTEX_BUFFER_HPP_INCLUDED

#include <fruitapp/fruit/mesh_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>


namespace fruitapp
{
namespace fruit
{
sge::renderer::vertex_buffer_ptr const
mesh_to_vertex_buffer(
	sge::renderer::device &,
	sge::renderer::vertex_declaration &,
	mesh const &);
}
}

#endif
