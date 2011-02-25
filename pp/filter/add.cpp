#include "add.hpp"
#include "../screen_vf/create_quad.hpp"
#include "../screen_vf/format.hpp"
#include "../texture/instance.hpp"
#include "../texture/manager.hpp"
#include "../texture/descriptor.hpp"
#include "../../media_path.hpp"
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
#include <sge/renderer/scoped_vertex_declaration.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/image/color/format.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/output.hpp>
#include <iostream>

fruitcut::pp::filter::add::add(
	sge::renderer::device_ptr const _renderer,
	texture::manager &_texture_manager,
	sge::renderer::dim2 const &_dimension)
:
	renderer_(
		_renderer),
	dimension_(
		_dimension),
	shader_(
		renderer_,
		media_path()
			/FCPPT_TEXT("shaders")
			/FCPPT_TEXT("add_vertex.glsl"),
		media_path()
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
			(sge::shader::sampler("tex2",sge::renderer::texture::planar_ptr()))), 
	quad_(
		screen_vf::create_quad(
			shader_,
			renderer_)),
	texture_manager_(
		_texture_manager)
{
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::filter::add::apply(
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
		shader_);

	sge::renderer::scoped_vertex_declaration const vb_declaration_context(
		renderer_,
		quad_.declaration());

	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		quad_.buffer());

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
			result->texture()->dim()));

	sge::renderer::scoped_target const scoped_target(
		renderer_,
		result->target()); 

	sge::renderer::scoped_block const block(
		renderer_);

	renderer_->render(
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			quad_.buffer()->size()),
		sge::renderer::nonindexed_primitive_type::triangle);

	return result;
}

fruitcut::pp::filter::add::~add()
{
}
