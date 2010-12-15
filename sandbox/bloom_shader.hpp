#ifndef FRUITCUT_SANDBOX_BLOOM_SHADER_HPP_INCLUDED
#define FRUITCUT_SANDBOX_BLOOM_SHADER_HPP_INCLUDED

#include "blur_filter.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/target_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/shader/object.hpp>
#include <boost/function.hpp>

namespace fruitcut
{
namespace sandbox
{
class bloom_shader
{
public:
	typedef
	boost::function<void ()>
	render_callback;

	explicit
	bloom_shader(
		sge::renderer::device_ptr);

	void
	render(
		render_callback const &);

	~bloom_shader();
private:
	sge::renderer::device_ptr renderer_;
	blur_filter blur_;
	sge::renderer::texture_ptr screen_texture_;
	sge::renderer::texture_ptr highlighted_texture_;
	sge::renderer::target_ptr screen_target_;
	sge::renderer::target_ptr highlighted_target_;
	sge::shader::object highlight_shader_;
	sge::shader::object combining_shader_;
	sge::renderer::vertex_buffer_ptr quad_vb1_;
	sge::renderer::vertex_buffer_ptr quad_vb2_;

	void
	render_to_texture(
		render_callback const &);

	void
	render_highlights();

	void 
	render_combined();
};
}
}

#endif
