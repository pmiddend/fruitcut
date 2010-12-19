#include "blur.hpp"
#include "../../screen_vf/create_quad.hpp"
#include "../../screen_vf/format.hpp"
#include "../../../media_path.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/no_depth_stencil_texture.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/sampler.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/shader/object.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <fcppt/assign/make_array.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/assert.hpp>
#include <cstddef>

fruitcut::sandbox::pp::filter::blur::blur(
	sge::renderer::device_ptr const _renderer,
	sge::renderer::dim2 const &texture_size,
	size_type const _iterations)
:
	renderer_(
		_renderer),
	iterations_(
		_iterations),
	textures_(
		fcppt::assign::make_array<sge::renderer::texture_ptr>
			(renderer_->create_texture(
					texture_size,
					sge::image::color::format::rgb8,
					sge::renderer::filter::linear,
					sge::renderer::resource_flags::none))
			(renderer_->create_texture(
					texture_size,
					sge::image::color::format::rgb8,
					sge::renderer::filter::linear,
					sge::renderer::resource_flags::none))),
	targets_(
		fcppt::assign::make_array<sge::renderer::target_ptr>
			(renderer_->create_target(
				textures_[1],
				sge::renderer::no_depth_stencil_texture()))
			(renderer_->create_target(
				textures_[0],
				sge::renderer::no_depth_stencil_texture()))),
	shaders_(
		fcppt::assign::make_array<sge::shader::object_ptr>
			(sge::shader::object_ptr(
				new sge::shader::object(
					renderer_,
					media_path()
						/FCPPT_TEXT("shaders")
						/FCPPT_TEXT("blur_vertical_vertex.glsl"),
					media_path()
						/FCPPT_TEXT("shaders")
						/FCPPT_TEXT("blur_vertical_fragment.glsl"),
					sge::shader::vf_to_string<screen_vf::format>(),
					fcppt::assign::make_container<sge::shader::variable_sequence>(
						sge::shader::variable(
							"texture_size",
							sge::shader::variable_type::const_,
							fcppt::math::dim::structure_cast<sge::renderer::vector2>(
								texture_size))),
					fcppt::assign::make_container<sge::shader::sampler_sequence>(
						sge::shader::sampler(
							"tex",
							textures_[0])))))
			(sge::shader::object_ptr(
				new sge::shader::object(
					renderer_,
					media_path()
						/FCPPT_TEXT("shaders")
						/FCPPT_TEXT("blur_horizontal_vertex.glsl"),
					media_path()
						/FCPPT_TEXT("shaders")
						/FCPPT_TEXT("blur_horizontal_fragment.glsl"),
					sge::shader::vf_to_string<screen_vf::format>(),
					fcppt::assign::make_container<sge::shader::variable_sequence>(
						sge::shader::variable(
							"texture_size",
							sge::shader::variable_type::const_,
							fcppt::math::dim::structure_cast<sge::renderer::vector2>(
								texture_size))),
					fcppt::assign::make_container<sge::shader::sampler_sequence>(
						sge::shader::sampler(
							"tex",
							textures_[1])))))),
	quads_(
		fcppt::assign::make_array<sge::renderer::vertex_buffer_ptr>
			(screen_vf::create_quad(
				*shaders_[0],
				renderer_))
			(screen_vf::create_quad(
				*shaders_[1],
				renderer_)))
{
	FCPPT_ASSERT(
		iterations_);
}

sge::renderer::texture_ptr const
fruitcut::sandbox::pp::filter::blur::apply(
	sge::renderer::texture_ptr const input)
{
	// Step 1: Set input texture of shader 0 to "input"
	// Step 2: Render using shader 0 to texture 1 and blur
	// Step 3: Update input texture of shader 0 to texture 1 (as it should be)
	// Step 4: Render using shader 1 to texture 0 and blur again
	// Proceed as neccessary, just flipping between 1 and 0 and
	// forgetting "input"
	shaders_[0]->update_texture(
		"tex",
		input);

	render(
		0);

	shaders_[0]->update_texture(
		"tex",
		textures_[0]);

	render(
		1);

	for(size_type i = 0; i < static_cast<size_type>(iterations_-1); ++i)
	{
		render(
			0);
		render(
			1);
	}

	return textures_[0];
}

fruitcut::sandbox::pp::filter::blur::~blur()
{
}

void
fruitcut::sandbox::pp::filter::blur::render(
	size_type const i)
{
	sge::shader::scoped scoped_shader(
		*shaders_[i]);

	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		quads_[i]);

	sge::renderer::scoped_target const target_(
		renderer_,
		targets_[i]); 

	sge::renderer::scoped_block const block_(
		renderer_);

	renderer_->render(
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			quads_[i]->size()),
		sge::renderer::nonindexed_primitive_type::triangle);
}
