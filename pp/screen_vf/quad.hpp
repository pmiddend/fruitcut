#ifndef FRUITCUT_PP_SCREEN_VF_QUAD_HPP_INCLUDED
#define FRUITCUT_PP_SCREEN_VF_QUAD_HPP_INCLUDED

#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/vertex_declaration_ptr.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/shader/object_fwd.hpp>

namespace fruitcut
{
namespace pp
{
namespace screen_vf
{
class quad
{
public:
	explicit
	quad(
		sge::renderer::device_ptr,
		sge::shader::object &);

	explicit
	quad();

	void
	render();

	~quad();
private:
	sge::renderer::device_ptr renderer_;
	sge::renderer::vertex_declaration_ptr declaration_;
	sge::renderer::vertex_buffer_ptr buffer_;
};
}
}
}

#endif
