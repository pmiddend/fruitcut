#ifndef FRUITCUT_OBJECT_HPP_INCLUDED
#define FRUITCUT_OBJECT_HPP_INCLUDED

#include "mesh.hpp"
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>

namespace fruitcut
{
class object
{
public:
	object(object const &) = delete;
	object &operator=(object const &) = delete;

	sge::renderer::texture_ptr texture_;
	mesh mesh_;
	sge::renderer::vertex_buffer_ptr vb;
};
}

#endif
