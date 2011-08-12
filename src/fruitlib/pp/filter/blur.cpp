#include <fruitlib/pp/filter/blur.hpp>
#include <fruitlib/pp/filter/manager.hpp>
#include <fruitlib/pp/texture/manager.hpp>
#include <fruitlib/pp/texture/instance.hpp>
#include <fruitlib/pp/texture/descriptor.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/shader/shader.hpp>
#include <fcppt/assign/make_array.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/assert.hpp>
#include <iostream>

fruitlib::pp::filter::blur::blur(
	sge::renderer::device &_renderer,
	filter::manager &_filter_manager,
	texture::manager &_texture_manager,
	sge::renderer::dim2 const &_texture_size,
	size_type const _iterations)
:
	renderer_(
		_renderer),
	filter_manager_(
		_filter_manager),
	texture_manager_(
		_texture_manager),
	texture_size_(
		_texture_size),
	iterations_(
		_iterations),
	shaders_(
		fcppt::assign::make_array<sge::shader::object*>
			(&(filter_manager_.lookup_shader(
				FCPPT_TEXT("blur_vertical"),
				fcppt::assign::make_container<sge::shader::variable_sequence>(
					sge::shader::variable(
						"texture_size",
						sge::shader::variable_type::uniform,
						sge::renderer::vector2())),
				fcppt::assign::make_container<sge::shader::sampler_sequence>(
					sge::shader::sampler(
						"tex",
						sge::renderer::texture::planar_ptr())))))
			(&(filter_manager_.lookup_shader(
				FCPPT_TEXT("blur_horizontal"),
				fcppt::assign::make_container<sge::shader::variable_sequence>(
					sge::shader::variable(
						"texture_size",
						sge::shader::variable_type::uniform,
						sge::renderer::vector2())),
				fcppt::assign::make_container<sge::shader::sampler_sequence>(
					sge::shader::sampler(
						"tex",
						sge::renderer::texture::planar_ptr()))))))
{
	FCPPT_ASSERT(
		iterations_);
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::filter::blur::apply(
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
					texture::depth_stencil_format::off)),
			texture_manager_.query(
				texture::descriptor(
					texture_size_,
					sge::image::color::format::rgb8,
					texture::depth_stencil_format::off))
		}};

	shaders_[0]->update_texture(
		"tex",
		input->texture());
	shaders_[1]->update_texture(
		"tex",
		instances[0]->texture());

	sge::shader::update_single_uniform(
		*shaders_[0],
		"texture_size",
		fcppt::math::dim::structure_cast<sge::renderer::vector2>(
				instances[0]->texture()->size()));
	sge::shader::update_single_uniform(
		*shaders_[1],
		"texture_size",
		fcppt::math::dim::structure_cast<sge::renderer::vector2>(
				instances[1]->texture()->size()));

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

fruitlib::pp::filter::blur::~blur()
{
}

void
fruitlib::pp::filter::blur::render(
	instance_array &textures,
	size_type const i)
{
	sge::shader::scoped scoped_shader(
		*shaders_[i],
		sge::shader::activate_everything());

	sge::renderer::scoped_target const target_(
		renderer_,
		*textures[i]->target());

	sge::renderer::scoped_block const block_(
		renderer_);

	filter_manager_.quad().render();
}
