#ifndef FRUITAPP_FRUIT_MESH_TO_VERTEX_BUFFER_HPP_INCLUDED
#define FRUITAPP_FRUIT_MESH_TO_VERTEX_BUFFER_HPP_INCLUDED

#include <fruitapp/fruit/mesh_fwd.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <sge/renderer/vertex/buffer_unique_ptr.hpp>
#include <sge/renderer/vertex/declaration_fwd.hpp>


namespace fruitapp
{
namespace fruit
{
sge::renderer::vertex::buffer_unique_ptr
mesh_to_vertex_buffer(
	sge::renderer::device::core &,
	sge::renderer::vertex::declaration &,
	mesh const &);
}
}

#endif
