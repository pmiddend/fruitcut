#include "add.hpp"
#include "../screen_vf/format.hpp"
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
	fcppt::filesystem::path const &_base_path,
	sge::renderer::device &_renderer,
	texture::manager &_texture_manager,
	sge::renderer::dim2 const &_dimension)
:
	renderer_(
		_renderer),
	dimension_(
		_dimension),
	shader_(
		sge::shader::object_parameters(
			renderer_,
			_base_path
				/FCPPT_TEXT("shaders")
				/FCPPT_TEXT("add_vertex.glsl"),
			_base_path
				/FCPPT_TEXT("shaders")
				/FCPPT_TEXT("add_fragment.glsl"),
			sge::shader::vf_to_string<screen_vf::format>(),
			fcppt::assign::make_container<sge::shader::variable_sequence>(
				sge::shader::variable(
					"texture_size",
					sge::shader::variable_type::uniform,
					sge::renderer::vector2())),
			fcppt::assign::make_container<sge::shader::sampler_sequence>
				(sge::shader::sampler("tex1",sge::renderer::texture::planar_ptr()))
				(sge::shader::sampler("tex2",sge::renderer::texture::planar_ptr())))), 
	quad_(
		renderer_,
		shader_),
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
		sge::shader::activation_method::with_textures);

	texture::counted_instance const result = 
		texture_manager_.query(
			texture::descriptor(
				dimension_,
				sge::image::color::format::rgb8,
				sge::renderer::texture::filter::linear,
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

	quad_.render();

	return result;
}

fruitcut::fruitlib::pp::filter::add::~add()
{
}
