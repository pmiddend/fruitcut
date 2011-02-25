#ifndef FRUITCUT_PP_SCREEN_VF_CREATE_QUAD_HPP_INCLUDED
#define FRUITCUT_PP_SCREEN_VF_CREATE_QUAD_HPP_INCLUDED

#include "declaration_buffer_pair.hpp"
#include <sge/shader/object_fwd.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>

namespace fruitcut
{
namespace pp
{
namespace screen_vf
{
declaration_buffer_pair const
create_quad(
	sge::shader::object &,
	sge::renderer::device_ptr);
}
}
}

#endif
