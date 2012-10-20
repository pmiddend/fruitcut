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
#include <sge/shader/scoped_pair.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/output.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


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
		sge::shader::vertex_program_path(
			_filter_manager.base_path().get() / FCPPT_TEXT("add.cg")),
		sge::shader::pixel_program_path(
			_filter_manager.base_path().get() / FCPPT_TEXT("add.cg")),
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
