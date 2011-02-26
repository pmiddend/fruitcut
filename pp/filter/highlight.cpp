#include "highlight.hpp"
#include "../../media_path.hpp"
#include "../screen_vf/format.hpp"
#include "../texture/manager.hpp"
#include "../texture/descriptor.hpp"
#include "../texture/instance.hpp"
#include <sge/image/color/format.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/shader/object.hpp>
#include <sge/shader/sampler.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>
#include <iostream>

fruitcut::pp::filter::highlight::highlight(
	sge::renderer::device_ptr const _renderer,
	texture::manager &_texture_manager,
	sge::renderer::dim2 const &_texture_size,
	sge::renderer::scalar const _threshold)
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
			/FCPPT_TEXT("highlight_vertex.glsl"),
		media_path()
			/FCPPT_TEXT("shaders")
			/FCPPT_TEXT("highlight_fragment.glsl"),
		sge::shader::vf_to_string<screen_vf::format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>
			(sge::shader::variable(
				"texture_size",
				sge::shader::variable_type::uniform,
				sge::renderer::vector2()))
			(sge::shader::variable(
				"threshold",
				sge::shader::variable_type::const_,
				_threshold)),
		fcppt::assign::make_container<sge::shader::sampler_sequence>(
			sge::shader::sampler(
				"tex",
				sge::renderer::texture::planar_ptr()))),
	quad_(
		renderer_,
		shader_)
{
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::filter::highlight::apply(
	texture::counted_instance const input)
{
	shader_.update_texture(
		"tex",
		input->texture());

	texture::counted_instance const result = 
		texture_manager_.query(
			texture::descriptor(
				texture_size_,
				sge::image::color::format::rgb8,
				sge::renderer::texture::filter::linear,
				texture::depth_stencil_format::off));

	sge::shader::scoped scoped_shader(
		shader_);

	shader_.update_uniform(
		"texture_size",
		fcppt::math::dim::structure_cast<sge::renderer::vector2>(
			result->texture()->dim()));

	sge::renderer::scoped_target const scoped_target(
		renderer_,
		result->target()); 

	sge::renderer::scoped_block const block(
		renderer_);

	quad_.render();

	return result;
}
