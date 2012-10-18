#include <fruitlib/pp/filter/highlight.hpp>
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


fruitlib::pp::filter::highlight::highlight(
	fruitlib::pp::filter::manager &_filter_manager,
	fruitlib::pp::texture::manager &_texture_manager,
	fruitlib::pp::filter::texture_size const &_texture_size,
	threshold_factor const &_threshold)
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
			_filter_manager.base_path().get() / FCPPT_TEXT("highlight.cg")),
		sge::shader::pixel_program_path(
			_filter_manager.base_path().get() / FCPPT_TEXT("highlight.cg")),
		_filter_manager.shader_cflags()),
	threshold_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"threshold"),
		_threshold.get()),
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
fruitlib::pp::filter::highlight::threshold(
	threshold_factor const &_factor)
{
	threshold_.set(
		_factor.get());
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::filter::highlight::apply(
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

fruitlib::pp::filter::highlight::~highlight()
{
}
