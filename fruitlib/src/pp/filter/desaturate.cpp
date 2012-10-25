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
#include <sge/shader/scoped_pair.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


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
		sge::shader::vertex_program_path(
			_filter_manager.base_path().get() / FCPPT_TEXT("desaturate.cg")),
		sge::shader::pixel_program_path(
			_filter_manager.base_path().get() / FCPPT_TEXT("desaturate.cg")),
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
			*(input->texture())));

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