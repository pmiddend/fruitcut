#include "add.hpp"
#include "manager.hpp"
#include "../texture/instance.hpp"
#include "../texture/manager.hpp"
#include "../texture/descriptor.hpp"
#include <sge/shader/shader.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/image/color/format.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/output.hpp>
#include <iostream>

fruitcut::fruitlib::pp::filter::add::add(
	sge::renderer::device &_renderer,
	filter::manager &_filter_manager,
	texture::manager &_texture_manager,
	sge::renderer::dim2 const &_dimension)
:
	renderer_(
		_renderer),
	filter_manager_(
		_filter_manager),
	dimension_(
		_dimension),
	shader_(
		filter_manager_.lookup_shader(
			FCPPT_TEXT("add"),
			fcppt::assign::make_container<sge::shader::variable_sequence>(
				sge::shader::variable(
					"texture_size",
					sge::shader::variable_type::uniform,
					sge::renderer::vector2())),
			fcppt::assign::make_container<sge::shader::sampler_sequence>
				(sge::shader::sampler("tex1",sge::renderer::texture::planar_ptr()))
				(sge::shader::sampler("tex2",sge::renderer::texture::planar_ptr())))),
	texture_manager_(
		_texture_manager)
{
}

fruitcut::fruitlib::pp::texture::counted_instance const
fruitcut::fruitlib::pp::filter::add::apply(
	texture::counted_instance const input1,
	texture::counted_instance const input2)
{
	shader_.update_texture(
		"tex1",
		input1->texture());

	shader_.update_texture(
		"tex2",
		input2->texture());

	sge::shader::scoped scoped_shader(
		shader_,
		sge::shader::activate_everything());

	texture::counted_instance const result = 
		texture_manager_.query(
			texture::descriptor(
				dimension_,
				sge::image::color::format::rgb8,
				texture::depth_stencil_format::off));

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

	return 
		result;
}

fruitcut::fruitlib::pp::filter::add::~add()
{
}
