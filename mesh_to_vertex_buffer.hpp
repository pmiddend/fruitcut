#ifndef FRUITCUT_MESH_TO_VERTEX_BUFFER_HPP_INCLUDED
#define FRUITCUT_MESH_TO_VERTEX_BUFFER_HPP_INCLUDED

#include "mesh_fwd.hpp"
#include <sge/shader/object_fwd.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>

namespace fruitcut
{
sge::renderer::vertex_buffer_ptr const
mesh_to_vertex_buffer(
	sge::renderer::device_ptr,
	sge::shader::object &,
	mesh const &);
}

#endif
