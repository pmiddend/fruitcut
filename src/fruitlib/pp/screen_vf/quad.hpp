#ifndef FRUITCUT_FRUITLIB_PP_SCREEN_VF_QUAD_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PP_SCREEN_VF_QUAD_HPP_INCLUDED

#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/vertex_declaration_ptr.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/shader/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace pp
{
namespace screen_vf
{
class quad
{
FCPPT_NONCOPYABLE(
	quad);
public:
	explicit
	quad(
		sge::renderer::device &,
		sge::shader::object &);

	explicit
	quad();

	void
	render();

	~quad();
private:
	sge::renderer::device &renderer_;
	sge::renderer::vertex_declaration_ptr declaration_;
	sge::renderer::vertex_buffer_ptr buffer_;
};
}
}
}
}

#endif
