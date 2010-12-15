#ifndef FRUITCUT_SANDBOX_BLUR_FILTER_HPP_INCLUDED
#define FRUITCUT_SANDBOX_BLUR_FILTER_HPP_INCLUDED

#include <sge/renderer/target_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/shader/object_ptr.hpp>
#include <fcppt/container/array.hpp>
#include <boost/function.hpp>

namespace fruitcut
{
namespace sandbox
{
class blur_filter
{
public:
	typedef
	boost::function<void ()>
	render_callback;

	explicit	
	blur_filter(
		sge::renderer::device_ptr,
		sge::renderer::dim2 const &);

	void
	render(
		render_callback const &);

	void
	iterate();

	sge::renderer::texture_ptr const
	texture() const;

	~blur_filter();
private:
	sge::renderer::device_ptr renderer_;

	// First textures, then targets!
	fcppt::container::array<sge::renderer::texture_ptr,2> textures_;
	fcppt::container::array<sge::renderer::target_ptr,2> targets_;
	fcppt::container::array<sge::shader::object_ptr,2> shaders_;
	fcppt::container::array<sge::renderer::vertex_buffer_ptr,2> quads_;
};
}
}

#endif
