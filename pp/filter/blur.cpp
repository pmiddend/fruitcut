#include "blur.hpp"
#include "../screen_vf/format.hpp"
#include "../../media_path.hpp"
#include "../texture/manager.hpp"
#include "../texture/instance.hpp"
#include "../texture/descriptor.hpp"
#include <sge/image/color/format.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/resource_flags_none.hpp>
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
#include <fcppt/assign/make_array.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/container/ptr/replace_unique_ptr.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <iostream>

fruitcut::pp::filter::blur::blur(
	sge::renderer::device &_renderer,
	texture::manager &_texture_manager,
	sge::renderer::dim2 const &_texture_size,
	size_type const _iterations)
:
	renderer_(
		_renderer),
	texture_manager_(
		_texture_manager),
	texture_size_(
		_texture_size),
	iterations_(
		_iterations),
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
							sge::shader::variable_type::uniform,
							sge::renderer::vector2())),
					fcppt::assign::make_container<sge::shader::sampler_sequence>(
						sge::shader::sampler(
							"tex",
							sge::renderer::texture::planar_ptr())))))
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
							sge::shader::variable_type::uniform,
							sge::renderer::vector2())),
					fcppt::assign::make_container<sge::shader::sampler_sequence>(
						sge::shader::sampler(
							"tex",
							sge::renderer::texture::planar_ptr())))))),
	quads_()
{
	FCPPT_ASSERT(
		iterations_);
	for(std::size_t i = 0; i < 1; ++i)
		fcppt::container::ptr::replace_unique_ptr(
			quads_,
			i,
			fcppt::make_unique_ptr<screen_vf::quad>(
				fcppt::ref(
					renderer_),
				fcppt::ref(
					*shaders_[i])));
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::filter::blur::apply(
	texture::counted_instance const input)
{
	// Step 1: Set input texture of shader 0 to "input"
	// Step 2: Render using shader 0 to texture 1 and blur
	// Step 3: Update input texture of shader 0 to texture 1 (as it should be)
	// Step 4: Render using shader 1 to texture 0 and blur again
	// Proceed as neccessary, just flipping between 1 and 0 and
	// forgetting "input"

	instance_array instances = 
		{{
			texture_manager_.query(
				texture::descriptor(
					texture_size_,
					sge::image::color::format::rgb8,
					sge::renderer::texture::filter::linear,
					texture::depth_stencil_format::off)),
			texture_manager_.query(
				texture::descriptor(
					texture_size_,
					sge::image::color::format::rgb8,
					sge::renderer::texture::filter::linear,
					texture::depth_stencil_format::off))
		}};

	shaders_[0]->update_texture(
		"tex",
		input->texture());
	shaders_[1]->update_texture(
		"tex",
		instances[0]->texture());

	{
		sge::renderer::glsl::scoped_program scoped_p(
			renderer_,
			shaders_[0]->program());
		shaders_[0]->update_uniform(
			"texture_size",
			fcppt::math::dim::structure_cast<sge::renderer::vector2>(
				instances[0]->texture()->size()));
	}
	{
		sge::renderer::glsl::scoped_program scoped_p(
			renderer_,
			shaders_[1]->program());
		shaders_[1]->update_uniform(
			"texture_size",
			fcppt::math::dim::structure_cast<sge::renderer::vector2>(
				instances[1]->texture()->size()));
	}

	render(
		instances,
		0);

	shaders_[0]->update_texture(
		"tex",
		instances[1]->texture());

	render(
		instances,
		1);

	for(size_type i = 0; i < static_cast<size_type>(iterations_-1); ++i)
	{
		render(
			instances,
			0);
		render(
			instances,
			1);
	}

	return instances[1];
}

fruitcut::pp::filter::blur::~blur()
{
}

void
fruitcut::pp::filter::blur::render(
	instance_array &textures,
	size_type const i)
{
	sge::shader::scoped scoped_shader(
		*shaders_[i]);

	sge::renderer::scoped_target const target_(
		renderer_,
		*textures[i]->target()); 

	sge::renderer::scoped_block const block_(
		renderer_);

	quads_[i].render();
}
