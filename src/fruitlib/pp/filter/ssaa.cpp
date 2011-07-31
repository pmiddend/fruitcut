#include "ssaa.hpp"
#include "manager.hpp"
#include "../texture/instance.hpp"
#include "../texture/manager.hpp"
#include <sge/image/color/format.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_declaration.hpp>
#include <sge/shader/shader.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <iostream>

fruitlib::pp::filter::ssaa::ssaa(
	sge::renderer::device &_renderer,
	filter::manager &_filter_manager,
	texture::manager &_texture_manager,
	sge::renderer::dim2 const &_texture_size)
:
	renderer_(
		_renderer),
	filter_manager_(
		_filter_manager),
	texture_manager_(
		_texture_manager),
	texture_size_(
		_texture_size),
	shader_(
		filter_manager_.lookup_shader(
			FCPPT_TEXT("ssaa"),
			fcppt::assign::make_container<sge::shader::variable_sequence>(
				sge::shader::variable(
					"texture_size",
					sge::shader::variable_type::uniform,
					sge::renderer::vector2())),
			fcppt::assign::make_container<sge::shader::sampler_sequence>(
				sge::shader::sampler(
					"tex",
					sge::renderer::texture::planar_ptr()))))
{
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::filter::ssaa::apply(
	fruitlib::pp::texture::counted_instance const input)
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

	sge::renderer::scoped_target const scoped_target(
		renderer_,
		*result->target()); 

	sge::renderer::scoped_block const block(
		renderer_);

	filter_manager_.quad().render();

	return result;
}

fruitlib::pp::filter::ssaa::~ssaa()
{
}
