#include "desaturate.hpp"
#include "../texture/descriptor.hpp"
#include "../texture/instance.hpp"
#include "../texture/manager.hpp"
#include "manager.hpp"
#include <sge/image/color/format.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/shader/shader.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>
#include <iostream>

fruitcut::fruitlib::pp::filter::desaturate::desaturate(
	sge::renderer::device &_renderer,
	filter::manager &_filter_manager,
	texture::manager &_texture_manager,
	sge::renderer::dim2 const &_texture_size,
	sge::renderer::scalar const _factor)
:
	renderer_(
		_renderer),
	filter_manager_(
		_filter_manager),
	texture_manager_(
		_texture_manager),
	texture_size_(
		_texture_size),
	factor_(
		_factor),
	shader_(
		filter_manager_.lookup_shader(
			FCPPT_TEXT("desaturate"),
			fcppt::assign::make_container<sge::shader::variable_sequence>
				(sge::shader::variable(
					"texture_size",
					sge::shader::variable_type::uniform,
					sge::renderer::vector2()))
				(sge::shader::variable(
					"factor",
					sge::shader::variable_type::uniform,
					_factor)),
			fcppt::assign::make_container<sge::shader::sampler_sequence>
				(sge::shader::sampler(
					"tex",
					sge::renderer::texture::planar_ptr()))))
{
}

void
fruitcut::fruitlib::pp::filter::desaturate::factor(
	sge::renderer::scalar const _factor)
{
	factor_ = 
		_factor;
}

fruitcut::fruitlib::pp::texture::counted_instance const
fruitcut::fruitlib::pp::filter::desaturate::apply(
	fruitcut::fruitlib::pp::texture::counted_instance const input)
{
	shader_.update_texture(
		"tex",
		input->texture());

	texture::counted_instance const result = 
		texture_manager_.query(
			texture::descriptor(
				texture_size_,
				sge::image::color::format::rgb8,
				texture::depth_stencil_format::off));

	sge::shader::scoped scoped_shader(
		shader_,
		sge::shader::activate_everything());

	shader_.update_uniform(
		"texture_size",
		fcppt::math::dim::structure_cast<sge::renderer::vector2>(
			result->texture()->size()));

	shader_.update_uniform(
		"factor",
		factor_);

	sge::renderer::scoped_target const scoped_target(
		renderer_,
		*result->target()); 

	sge::renderer::scoped_block const block(
		renderer_);

	filter_manager_.quad().render();

	return result;
}
