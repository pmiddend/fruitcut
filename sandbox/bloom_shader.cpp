#include "bloom_shader.hpp"
#include "../media_path.hpp"
#include "screen_vf/format.hpp"
#include "screen_vf/create_quad.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/filter/point.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/no_depth_stencil_texture.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/default_target.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/shader/sampler.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/image/color/format.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/assign/make_container.hpp>

namespace
{
sge::renderer::dim2 const texture_size(256,256);
}

fruitcut::sandbox::bloom_shader::bloom_shader(
	sge::renderer::device_ptr const _renderer,
	render_callback const &_render_callback)
:
	renderer_(
		_renderer),
	render_callback_(
		_render_callback),
	screen_texture_(
		renderer_->create_texture(
			fcppt::math::dim::structure_cast<sge::renderer::dim2>(
				renderer_->screen_size()),
			sge::image::color::format::rgb8,
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none)),
	highlighted_texture_(
		renderer_->create_texture(
			texture_size,
			sge::image::color::format::rgb8,
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none)),
	blurred_vertical_texture_(
		renderer_->create_texture(
			texture_size,
			sge::image::color::format::rgb8,
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none)),
	blurred_both_texture_(
		renderer_->create_texture(
			texture_size,
			sge::image::color::format::rgb8,
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none)),


	screen_target_(
		renderer_->create_target(
			screen_texture_,
			sge::renderer::no_depth_stencil_texture())),


	highlighted_target_(
		renderer_->create_target(
			highlighted_texture_,
			sge::renderer::no_depth_stencil_texture())),


	blurred_vertical_target_(
		renderer_->create_target(
			blurred_vertical_texture_,
			sge::renderer::no_depth_stencil_texture())),



	blurred_both_target_(
		renderer_->create_target(
			blurred_both_texture_,
			sge::renderer::no_depth_stencil_texture())),


	highlight_shader_(
		renderer_,
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("bloom")/FCPPT_TEXT("highlight_vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("bloom")/FCPPT_TEXT("highlight_fragment.glsl"),
		sge::shader::vf_to_string<screen_vf::format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>(
			sge::shader::variable(
				"texture_size",
				sge::shader::variable_type::const_,
				fcppt::math::dim::structure_cast<sge::renderer::vector2>(
					texture_size))),
		fcppt::assign::make_container<sge::shader::sampler_sequence>(
			sge::shader::sampler(
				"texture",
				screen_texture_))),
	blur_shader_vertical_(
		renderer_,
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("bloom")/FCPPT_TEXT("blur_vertical_vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("bloom")/FCPPT_TEXT("blur_vertical_fragment.glsl"),
		sge::shader::vf_to_string<screen_vf::format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>(
			sge::shader::variable(
				"texture_size",
				sge::shader::variable_type::const_,
				fcppt::math::dim::structure_cast<sge::renderer::vector2>(
					texture_size))),
		fcppt::assign::make_container<sge::shader::sampler_sequence>(
			sge::shader::sampler(
				"texture",
				highlighted_texture_))),
	blur_shader_horizontal_(
		renderer_,
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("bloom")/FCPPT_TEXT("blur_horizontal_vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("bloom")/FCPPT_TEXT("blur_horizontal_fragment.glsl"),
		sge::shader::vf_to_string<screen_vf::format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>(
			sge::shader::variable(
				"texture_size",
				sge::shader::variable_type::const_,
				fcppt::math::dim::structure_cast<sge::renderer::vector2>(
					texture_size))),
		fcppt::assign::make_container<sge::shader::sampler_sequence>(
			sge::shader::sampler(
				"texture",
				blurred_vertical_texture_))),
	combining_shader_(
		renderer_,
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("bloom")/FCPPT_TEXT("combining_vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("bloom")/FCPPT_TEXT("combining_fragment.glsl"),
		sge::shader::vf_to_string<screen_vf::format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>(
			sge::shader::variable(
				"texture_size",
				sge::shader::variable_type::const_,
				fcppt::math::dim::structure_cast<sge::renderer::vector2>(
					renderer_->screen_size()))),
		fcppt::assign::make_container<sge::shader::sampler_sequence>
			(sge::shader::sampler(
				"original",
				screen_texture_))
			(sge::shader::sampler(
				"blurred",
				blurred_both_texture_))),

	quad_vb1_(
		screen_vf::create_quad(
			highlight_shader_,
			renderer_)),
	quad_vb2_(
		screen_vf::create_quad(
			blur_shader_vertical_,
			renderer_)),
	quad_vb3_(
		screen_vf::create_quad(
			blur_shader_horizontal_,
			renderer_)),
	quad_vb4_(
		screen_vf::create_quad(
			combining_shader_,
			renderer_))
{
}

void
fruitcut::sandbox::bloom_shader::render()
{
	// Step 1: Render scene to screen_texture
	{
		sge::renderer::scoped_target const target_(
			renderer_,
			screen_target_); 

		sge::renderer::scoped_block const block_(
			renderer_);

		render_callback_();
	}

	// Step 2: Transfer screen_texture to highlighted_texture_ and do
	// highlighting on it
	{
		sge::shader::scoped scoped_shader(
			highlight_shader_);

		sge::renderer::scoped_vertex_buffer const scoped_vb_(
			renderer_,
			quad_vb1_);

		sge::renderer::scoped_target const target_(
			renderer_,
			highlighted_target_); 

		sge::renderer::scoped_block const block_(
			renderer_);

		renderer_->render(
			sge::renderer::first_vertex(
				0),
			sge::renderer::vertex_count(
				quad_vb1_->size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}

	// Step 3: Blur the highlighted texture vertically
	{
		sge::shader::scoped scoped_shader(
			blur_shader_vertical_);

		sge::renderer::scoped_vertex_buffer const scoped_vb_(
			renderer_,
			quad_vb2_);

		sge::renderer::scoped_target const target_(
			renderer_,
			blurred_vertical_target_); 

		sge::renderer::scoped_block const block_(
			renderer_);

		renderer_->render(
			sge::renderer::first_vertex(
				0),
			sge::renderer::vertex_count(
				quad_vb2_->size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}

	// Step 4: Blur the highlighted texture horizontally
	{
		sge::shader::scoped scoped_shader(
			blur_shader_horizontal_);

		sge::renderer::scoped_vertex_buffer const scoped_vb_(
			renderer_,
			quad_vb3_);

		sge::renderer::scoped_target const target_(
			renderer_,
			blurred_both_target_); 

		sge::renderer::scoped_block const block_(
			renderer_);

		renderer_->render(
			sge::renderer::first_vertex(
				0),
			sge::renderer::vertex_count(
				quad_vb3_->size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}

	// Step 4: Combine screen texture and blurred_texture to final image
	{
		sge::shader::scoped scoped_shader(
			combining_shader_);

		sge::renderer::scoped_vertex_buffer const scoped_vb_(
			renderer_,
			quad_vb4_);

		sge::renderer::scoped_target const target_(
			renderer_,
			sge::renderer::default_target()); 

		sge::renderer::scoped_block const block_(
			renderer_);

		renderer_->render(
			sge::renderer::first_vertex(
				0),
			sge::renderer::vertex_count(
				quad_vb4_->size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}
}

fruitcut::sandbox::bloom_shader::~bloom_shader()
{
}
