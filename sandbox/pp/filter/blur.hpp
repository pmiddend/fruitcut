#ifndef FRUITCUT_SANDBOX_PP_FILTER_BLUR_HPP_INCLUDED
#define FRUITCUT_SANDBOX_PP_FILTER_BLUR_HPP_INCLUDED

#include "unary.hpp"
#include <sge/renderer/target_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/shader/object_ptr.hpp>
#include <fcppt/container/array.hpp>
#include <cstddef>

namespace fruitcut
{
namespace sandbox
{
namespace pp
{
namespace filter
{
class blur
:
	public unary
{
public:
	typedef
	std::size_t
	size_type;

	explicit	
	blur(
		sge::renderer::device_ptr,
		sge::renderer::dim2 const &,
		size_type iterations);

	sge::renderer::texture_ptr const
	apply(
		sge::renderer::texture_ptr);

	~blur();
private:
	sge::renderer::device_ptr renderer_;
	size_type const iterations_;

	// First textures, then targets!
	fcppt::container::array<sge::renderer::texture_ptr,2> textures_;
	fcppt::container::array<sge::renderer::target_ptr,2> targets_;
	fcppt::container::array<sge::shader::object_ptr,2> shaders_;
	fcppt::container::array<sge::renderer::vertex_buffer_ptr,2> quads_;

	void
	render(
		size_type);
};
}
}
}
}

#endif
