#include <fruitlib/pp/filter/desaturate.hpp>
#include <fruitlib/pp/filter/manager.hpp>
#include <fruitlib/pp/texture/descriptor.hpp>
#include <fruitlib/pp/texture/instance.hpp>
#include <fruitlib/pp/texture/manager.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/context/scoped_core.hpp>
#include <sge/renderer/target/offscreen.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/shader/istream_ref.hpp>
#include <sge/shader/scoped_pair.hpp>
#include <fcppt/make_ref.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <sstream>
#include <fcppt/config/external_end.hpp>


namespace
{
char const desaturate_source[] =
	"struct vertex_outputs\n"
	"{\n"
	"	float4 position : POSITION;\n"
	"	float2 texture_coordinate : TEXCOORD0;\n"
	"};\n"
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
	"float\n"
	"luminance(\n"
	"	float3 color)\n"
	"{\n"
	"	return\n"
	"		dot(\n"
	"			color,\n"
	"			float3(\n"
	"				0.212,\n"
	"				0.716,\n"
	"				0.072));\n"
	"}\n"
	"float4\n"
	"pixel_main(\n"
	"	vertex_outputs vertex_data,\n"
	"	uniform float scaling,\n"
	"	uniform sampler2D input_texture)\n"
	"	: COLOR\n"
	"{\n"
	"	float4 texture_value =\n"
	"		tex2D(\n"
	"			  input_texture,\n"
	"			  vertex_data.texture_coordinate);\n"
	"	float gray =\n"
	"		luminance(\n"
	"			texture_value.xyz);\n"
	"	return\n"
	"		float4(\n"
	"		   lerp(\n"
	"				gray.xxx,\n"
	"				texture_value.xyz,\n"
	"				scaling),\n"
	"			1.0);\n"
	"}\n";
}

fruitlib::pp::filter::desaturate::desaturate(
	fruitlib::pp::filter::manager &_filter_manager,
	fruitlib::pp::texture::manager &_texture_manager,
	fruitlib::pp::filter::texture_size const &_texture_size,
	scaling_factor const &_scaling)
:
	filter_manager_(
		_filter_manager),
	texture_manager_(
		_texture_manager),
	texture_size_(
		_texture_size),
	shader_(
		_filter_manager.shader_context(),
		_filter_manager.quad().vertex_declaration(),
		sge::shader::vertex_program_stream(
			sge::shader::istream_ref(
				*fcppt::make_unique_ptr<
					std::istringstream
				>(
					std::string(
						desaturate_source
					)
				)
			)
		),
		sge::shader::pixel_program_stream(
			sge::shader::istream_ref(
				*fcppt::make_unique_ptr<
					std::istringstream
				>(
					std::string(
						desaturate_source
					)
				)
			)
		),
		_filter_manager.shader_cflags()),
	scaling_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"scaling"),
		_scaling.get()),
	texture_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"input_texture"),
		shader_,
		filter_manager_.renderer(),
		sge::shader::parameter::planar_texture::optional_value())
{
}

void
fruitlib::pp::filter::desaturate::scaling(
	scaling_factor const &_factor)
{
	scaling_.set(
		_factor.get());
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::filter::desaturate::apply(
	fruitlib::pp::texture::counted_instance const input)
{
	texture_.set(
		sge::shader::parameter::planar_texture::optional_value(
			fcppt::make_ref(
				*(input->texture()))));

	fruitlib::pp::texture::counted_instance const result =
		texture_manager_.query(
			fruitlib::pp::texture::descriptor(
				texture_size_.get(),
				sge::image::color::format::rgb8,
				texture::depth_stencil_format::off));

	sge::renderer::context::scoped_core scoped_context(
		filter_manager_.renderer(),
		result->target());

	sge::shader::scoped_pair scoped_shader(
		scoped_context.get(),
		shader_);

	filter_manager_.quad().render(
		scoped_context.get());

	return result;
}

fruitlib::pp::filter::desaturate::~desaturate()
{
}
