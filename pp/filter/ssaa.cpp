#include "ssaa.hpp"
#include "../screen_vf/create_quad.hpp"
#include "../screen_vf/format.hpp"
#include "../../media_path.hpp"
#include "../texture/instance.hpp"
#include "../texture/manager.hpp"
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/sampler.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/shader/object.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/image/color/format.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/structure_cast.hpp>

fruitcut::pp::filter::ssaa::ssaa(
	sge::renderer::device_ptr const _renderer,
	texture::manager &_texture_manager,
	sge::renderer::dim2 const &_texture_size)
:
	renderer_(
		_renderer),
	texture_manager_(
		_texture_manager),
	texture_size_(
		_texture_size),
	shader_(
		renderer_,
		media_path()
			/FCPPT_TEXT("shaders")
			/FCPPT_TEXT("ssaa_vertex.glsl"),
		media_path()
			/FCPPT_TEXT("shaders")
			/FCPPT_TEXT("ssaa_fragment.glsl"),
		sge::shader::vf_to_string<screen_vf::format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>(
			sge::shader::variable(
				"texture_size",
				sge::shader::variable_type::const_,
				fcppt::math::dim::structure_cast<sge::renderer::vector2>(
					texture_size_))),
		fcppt::assign::make_container<sge::shader::sampler_sequence>(
			sge::shader::sampler(
				"tex",
				sge::renderer::texture_ptr()))),
	quad_(
		screen_vf::create_quad(
			shader_,
			renderer_))
{
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::filter::ssaa::apply(
	fruitcut::pp::texture::counted_instance const input)
{
	shader_.update_texture(
		"tex",
		input->texture());

	texture::counted_instance const result = 
		texture_manager_.query(
			texture::descriptor(
				texture_size_,
				sge::image::color::format::rgb8,
				sge::renderer::filter::linear));

	sge::shader::scoped scoped_shader(
		shader_);

	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		quad_);

	sge::renderer::scoped_target const scoped_target(
		renderer_,
		result->target()); 

	sge::renderer::scoped_block const block(
		renderer_);

	renderer_->render(
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			quad_->size()),
		sge::renderer::nonindexed_primitive_type::triangle);

	return result;
}

fruitcut::pp::filter::ssaa::~ssaa()
{
}
