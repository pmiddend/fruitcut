#include <fruitlib/pp/filter/add.hpp>
#include <fruitlib/pp/filter/manager.hpp>
#include <fruitlib/pp/texture/descriptor.hpp>
#include <fruitlib/pp/texture/instance.hpp>
#include <fruitlib/pp/texture/manager.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/context/scoped_core.hpp>
#include <sge/renderer/target/offscreen.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/shader/istream_ref.hpp>
#include <sge/shader/scoped_pair.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/output.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <sstream>
#include <fcppt/config/external_end.hpp>


namespace
{
char const add_filter_source[] =
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
	"float4\n"
	"pixel_main(\n"
	"	vertex_outputs vertex_data,\n"
	"	uniform sampler2D input_texture1,\n"
	"	uniform sampler2D input_texture2)\n"
	"	: COLOR\n"
	"{\n"
	"	return\n"
	"		tex2D(\n"
	"			  input_texture1,\n"
	"			  vertex_data.texture_coordinate)+\n"
	"		tex2D(\n"
	"			  input_texture2,\n"
	"			  vertex_data.texture_coordinate);\n"
	"}\n";
}

fruitlib::pp::filter::add::add(
	fruitlib::pp::filter::manager &_filter_manager,
	fruitlib::pp::texture::manager &_texture_manager,
	fruitlib::pp::filter::texture_size const &_dimension)
:
	filter_manager_(
		_filter_manager),
	texture_manager_(
		_texture_manager),
	dimension_(
		_dimension),
	shader_(
		_filter_manager.shader_context(),
		_filter_manager.quad().vertex_declaration(),
		sge::shader::vertex_program_stream(
			sge::shader::istream_ref(
				*fcppt::make_unique_ptr<
					std::istringstream
				>(
					std::string(
						add_filter_source
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
						add_filter_source
					)
				)
			)
		),
		_filter_manager.shader_cflags()),
	texture1_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"input_texture1"),
		shader_,
		filter_manager_.renderer(),
		sge::shader::parameter::planar_texture::optional_value()),
	texture2_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"input_texture2"),
		shader_,
		filter_manager_.renderer(),
		sge::shader::parameter::planar_texture::optional_value())
{
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::filter::add::apply(
	fruitlib::pp::texture::counted_instance const input1,
	fruitlib::pp::texture::counted_instance const input2)
{
	texture1_.set(
		sge::shader::parameter::planar_texture::optional_value(
			*(input1->texture())));

	texture2_.set(
		sge::shader::parameter::planar_texture::optional_value(
			*(input2->texture())));

	fruitlib::pp::texture::counted_instance const result =
		texture_manager_.query(
			fruitlib::pp::texture::descriptor(
				dimension_.get(),
				sge::image::color::format::rgb8,
				fruitlib::pp::texture::depth_stencil_format::off));

	sge::renderer::context::scoped_core scoped_context(
		filter_manager_.renderer(),
		result->target());

	sge::shader::scoped_pair scoped_shader(
		scoped_context.get(),
		shader_);

	filter_manager_.quad().render(
		scoped_context.get());

	return
		result;
}

fruitlib::pp::filter::add::~add()
{
}
