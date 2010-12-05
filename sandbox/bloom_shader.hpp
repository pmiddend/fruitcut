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
	sge::renderer::texture_ptr tone_mapped_texture_;
	//sge::renderer::texture_ptr blur_texture_;
	sge::renderer::target_ptr screen_target_;
	sge::renderer::target_ptr tone_mapped_target_;
	//sge::renderer::target_ptr blur_target_;
	sge::renderer::vertex_buffer_ptr quad_vb_;
	sge::renderer::vertex_buffer_ptr quad_vb2_;
	//sge::renderer::vertex_buffer_ptr quad_vb3_;
	sge::shader::object tone_mapped_shader_;
	sge::shader::object combining_shader_;
	//sge::shader::object blur_shader_;
};
}
}

#endif
