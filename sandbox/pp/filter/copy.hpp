#ifndef FRUITCUT_SANDBOX_PP_FILTER_COPY_HPP_INCLUDED
#define FRUITCUT_SANDBOX_PP_FILTER_COPY_HPP_INCLUDED

#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/shader/object.hpp>

namespace fruitcut
{
namespace sandbox
{
namespace pp
{
namespace filter
{
// NOTE: This is not a "real" filter, it's only used in system to
// render a plain texture to the framebuffer in the final render
// step. It cannot be derived from base (or unary, binary, ...)
// because system holds it by value.
class copy
{
public:
	explicit
	copy(
		sge::renderer::device_ptr);

	void
	apply(
		sge::renderer::texture_ptr);
private:
	sge::renderer::device_ptr renderer_;
	sge::shader::object shader_;
	sge::renderer::vertex_buffer_ptr quad_;
};
}
}
}
}

#endif
