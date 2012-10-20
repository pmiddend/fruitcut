#include <fruitlib/pp/filter/manager.hpp>
#include <fruitlib/pp/filter/ssaa.hpp>
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
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitlib::pp::filter::ssaa::ssaa(
	fruitlib::pp::filter::manager &_filter_manager,
	fruitlib::pp::texture::manager &_texture_manager,
	fruitlib::pp::filter::texture_size const &_texture_size)
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
			_filter_manager.base_path().get() / FCPPT_TEXT("ssaa.cg")),
		sge::shader::pixel_program_path(
			_filter_manager.base_path().get() / FCPPT_TEXT("ssaa.cg")),
		_filter_manager.shader_cflags()),
	texture_size_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"texture_size"),
		fcppt::math::dim::structure_cast<fruitlib::pp::filter::ivec2_parameter::vector_type>(
			texture_size_.get())),
	texture_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"tex"),
		shader_,
		filter_manager_.renderer(),
		sge::shader::parameter::planar_texture::optional_value())
{
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::filter::ssaa::apply(
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

	texture_size_parameter_.set(
		fcppt::math::dim::structure_cast<fruitlib::pp::filter::ivec2_parameter::vector_type>(
			result->texture()->size()));

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

fruitlib::pp::filter::ssaa::~ssaa()
{
}
