#include <fruitlib/pp/filter/blur.hpp>
#include <fruitlib/pp/filter/manager.hpp>
#include <fruitlib/pp/texture/descriptor.hpp>
#include <fruitlib/pp/texture/instance.hpp>
#include <fruitlib/pp/texture/manager.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/context/core.hpp>
#include <sge/renderer/context/scoped_core.hpp>
#include <sge/renderer/target/offscreen.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/shader/scoped_pair.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/assert/pre.hpp>
#include <fcppt/cast/int_to_float_fun.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/math/dim/to_vector.hpp>
#include <fcppt/math/vector/null.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/config/external_begin.hpp>
#include <array>
#include <cstddef>
#include <memory>
#include <sstream>
#include <string>
#include <fcppt/config/external_end.hpp>

namespace
{
char const blur_h_source_code[] =
	"struct vertex_outputs\n"
	"{\n"
	"	float4 position : POSITION;\n"
	"	float2 texture_coordinate : TEXCOORD0;\n"
	"};\n"
	"#define NEIGHBORHOOD_SIZE 5\n"
	"float distributions[NEIGHBORHOOD_SIZE] = { 0.0280412,0.233924,0.474425,0.233924,0.0280412 };\n"
	"vertex_outputs\n"
	"vertex_main(\n"
	"	in float2 position : POSITION)\n"
	"{\n"
	"	vertex_outputs outs;\n"
	"	outs.position = float4(position.xy,0.0,1.0);\n"
	"	outs.texture_coordinate = (position.xy + float2(1.0,1.0))/2.0;\n"
	"	return\n"
	"		outs;\n"
	"}\n"
	"float4\n"
	"pixel_main(\n"
	"	vertex_outputs vertex_data,\n"
	"	uniform float2 texture_size,\n"
	"	uniform sampler2D input_texture)\n"
	"	: COLOR\n"
	"{\n"
	"	float4 sum =\n"
	"		0.0;\n"
	"	for(int i = 0; i < NEIGHBORHOOD_SIZE; ++i)\n"
	"	{\n"
	"		float2\n"
	"			current_absolute_texcoord =\n"
	"				texture_size * vertex_data.texture_coordinate,\n"
	"			current_texcoord =\n"
	"				(current_absolute_texcoord - float2((float)(i - 2),0.0)) /\n"
	"				texture_size;\n"
	"		sum +=\n"
	"			distributions[i] *\n"
	"			tex2D(\n"
	"				input_texture,\n"
	"				current_texcoord);\n"
	"	}\n"
	"	return\n"
	"		sum;\n"
	"}\n";

char const blur_v_source_code[] =
	"struct vertex_outputs\n"
	"{\n"
	"	float4 position : POSITION;\n"
	"	float2 texture_coordinate : TEXCOORD0;\n"
	"};\n"
	"#define NEIGHBORHOOD_SIZE 5\n"
	"float distributions[NEIGHBORHOOD_SIZE] = { 0.0280412,0.233924,0.474425,0.233924,0.0280412 };\n"
	"vertex_outputs\n"
	"vertex_main(\n"
	"	in float2 position : POSITION)\n"
	"{\n"
	"	vertex_outputs outs;\n"
	"	outs.position = float4(position.xy,0.0,1.0);\n"
	"	outs.texture_coordinate = (position.xy + float2(1.0,1.0))/2.0;\n"
	"	return\n"
	"		outs;\n"
	"}\n"
	"float4\n"
	"pixel_main(\n"
	"	vertex_outputs vertex_data,\n"
	"	uniform float2 texture_size,\n"
	"	uniform sampler2D input_texture)\n"
	"	: COLOR\n"
	"{\n"
	"	float4 sum =\n"
	"		0.0;\n"
	"	for(int i = 0; i < NEIGHBORHOOD_SIZE; ++i)\n"
	"	{\n"
	"		float2\n"
	"			current_absolute_texcoord =\n"
	"				texture_size * vertex_data.texture_coordinate,\n"
	"			current_texcoord =\n"
	"				(current_absolute_texcoord - float2(0.0,(float)(i - 2))) /\n"
	"				texture_size;\n"
	"		sum +=\n"
	"			distributions[i] *\n"
	"			tex2D(\n"
	"				input_texture,\n"
	"				current_texcoord);\n"
	"	}\n"
	"	return\n"
	"		sum;\n"
	"}\n";
}

fruitlib::pp::filter::blur::blur(
	fruitlib::pp::filter::manager &_filter_manager,
	fruitlib::pp::texture::manager &_texture_manager,
	fruitlib::pp::filter::texture_size const &_texture_size,
	fruitlib::pp::filter::iterations const &_iterations)
:
	filter_manager_(
		_filter_manager),
	texture_manager_(
		_texture_manager),
	texture_size_(
		_texture_size),
	iterations_(
		_iterations),
	shaders_(),
	planar_textures_(),
	texture_sizes_()
{
	FCPPT_ASSERT_PRE(
		iterations_.get());

	std::array<char const *,2> sources
	{{
		blur_h_source_code,
		blur_v_source_code
	}};

	// TODO: Initialize directly
	for(
		std::size_t i = 0;
		i < 2;
		++i)
	{
		shaders_[
			i
		] =
			std::make_unique<sge::shader::pair>(
				_filter_manager.shader_context(),
				_filter_manager.quad().vertex_declaration(),
				sge::shader::vertex_program_stream(
					*fcppt::make_unique_ptr<std::istringstream>(
						std::string(
							sources[i]))),
				sge::shader::pixel_program_stream(
					*fcppt::make_unique_ptr<std::istringstream>(
						std::string(
							sources[i]))),
				_filter_manager.shader_cflags());

		planar_textures_[
			i
		] =
			std::make_unique<sge::shader::parameter::planar_texture>(
				shaders_[i]->pixel_program(),
				sge::shader::parameter::name(
					"input_texture"),
				*shaders_[i],
				_filter_manager.renderer(),
				sge::shader::parameter::planar_texture::optional_value());

		texture_sizes_[
			i
		] =
			std::make_unique<vec2_parameter>(
				shaders_[i]->pixel_program(),
				sge::shader::parameter::name(
					"texture_size"),
				fcppt::math::vector::null<
					sge::renderer::vector2
				>()
			);
	}
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::filter::blur::apply(
	fruitlib::pp::texture::counted_instance const input)
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
			fruitlib::pp::texture::descriptor(
				texture_size_.get(),
				sge::image::color::format::rgb8,
				fruitlib::pp::texture::depth_stencil_format::off)),
		texture_manager_.query(
			fruitlib::pp::texture::descriptor(
				texture_size_.get(),
				sge::image::color::format::rgb8,
				fruitlib::pp::texture::depth_stencil_format::off))
	}};

	planar_textures_[0u]->set(
		sge::shader::parameter::planar_texture::optional_value(
			*input->texture()));

	planar_textures_[1u]->set(
		sge::shader::parameter::planar_texture::optional_value(
			*(instances[0]->texture())));

	texture_sizes_[0u]->set(
		fcppt::math::vector::structure_cast<sge::renderer::vector2, fcppt::cast::int_to_float_fun>(
			fcppt::math::dim::to_vector(
				input->texture()->size())));

	texture_sizes_[1u]->set(
		fcppt::math::vector::structure_cast<sge::renderer::vector2, fcppt::cast::int_to_float_fun>(
			fcppt::math::dim::to_vector(
				instances[0]->texture()->size())));

	this->render(
		instances,
		instance_index(
			0u));

	planar_textures_[0u]->set(
		sge::shader::parameter::planar_texture::optional_value(
			*(instances[1]->texture())));

	this->render(
		instances,
		instance_index(
			1u));

	for(
		fruitlib::pp::filter::iterations i(
			0u);
		i < fruitlib::pp::filter::iterations(iterations_.get()-1);
		++i)
	{
		this->render(
			instances,
			instance_index(
				0u));
		this->render(
			instances,
			instance_index(
				1u));
	}

	return
		instances[1u];
}

fruitlib::pp::filter::blur::~blur()
{
}

void
fruitlib::pp::filter::blur::render(
	instance_array &_textures,
	instance_index const &_instance)
{
	sge::renderer::context::scoped_core scoped_context(
		filter_manager_.renderer(),
		_textures[_instance.get()]->target());

	sge::shader::scoped_pair scoped_shader(
		scoped_context.get(),
		*shaders_[_instance.get()]);

	filter_manager_.quad().render(
		scoped_context.get());
}
