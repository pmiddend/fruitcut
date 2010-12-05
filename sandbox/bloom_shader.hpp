#ifndef FRUITCUT_SANDBOX_BLOOM_SHADER_HPP_INCLUDED
#define FRUITCUT_SANDBOX_BLOOM_SHADER_HPP_INCLUDED

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
		sge::renderer::device_ptr,
		render_callback const &);

	void
	render();

	~bloom_shader();
private:
	sge::renderer::device_ptr renderer_;
	render_callback const render_callback_;
	sge::renderer::texture_ptr screen_texture_;
	sge::renderer::texture_ptr highlighted_texture_;
	sge::renderer::texture_ptr blurred_vertical_texture_;
	sge::renderer::texture_ptr blurred_both_texture_;
	sge::renderer::target_ptr screen_target_;
	sge::renderer::target_ptr highlighted_target_;
	sge::renderer::target_ptr blurred_vertical_target_;
	sge::renderer::target_ptr blurred_both_target_;
	sge::renderer::vertex_buffer_ptr quad_vb1_;
	sge::renderer::vertex_buffer_ptr quad_vb2_;
	sge::renderer::vertex_buffer_ptr quad_vb3_;
	sge::renderer::vertex_buffer_ptr quad_vb4_;
	sge::shader::object highlight_shader_;
	sge::shader::object blur_shader_vertical_;
	sge::shader::object blur_shader_horizontal_;
	sge::shader::object combining_shader_;
};
}
}

#endif
