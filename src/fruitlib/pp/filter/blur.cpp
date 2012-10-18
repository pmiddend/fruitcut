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
#include <fcppt/ref.hpp>
#include <fcppt/assert/pre.hpp>
#include <fcppt/assign/make_array.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/ptr/replace_unique_ptr.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


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
	planar_textures_()
{
	FCPPT_ASSERT_PRE(
		iterations_.get());

	fcppt::container::array<boost::filesystem::path,2> filenames =
		{{
				boost::filesystem::path(
					FCPPT_TEXT("blur_h.cg")),
				boost::filesystem::path(
					FCPPT_TEXT("blur_v.cg")),
		}};

	for(
		std::size_t i = 0;
		i < 2;
		++i)
	{
		fcppt::container::ptr::replace_unique_ptr(
			shaders_,
			i,
			fcppt::make_unique_ptr<sge::shader::pair>(
				fcppt::ref(
					_filter_manager.shader_context()),
				fcppt::ref(
					_filter_manager.quad().vertex_declaration()),
				sge::shader::vertex_program_path(
					_filter_manager.base_path().get() / filenames[i]),
				sge::shader::pixel_program_path(
					_filter_manager.base_path().get() / filenames[i]),
				_filter_manager.shader_cflags()));

		fcppt::container::ptr::replace_unique_ptr(
			planar_textures_,
			i,
			fcppt::make_unique_ptr<sge::shader::parameter::planar_texture>(
				fcppt::ref(
					shaders_[i].pixel_program()),
				sge::shader::parameter::name(
					"input_texture"),
				fcppt::ref(
					shaders_[i]),
				fcppt::ref(
					_filter_manager.renderer()),
				sge::shader::parameter::planar_texture::optional_value()));
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

	planar_textures_[0u].set(
		sge::shader::parameter::planar_texture::optional_value(
			*input->texture()));

	planar_textures_[1u].set(
		sge::shader::parameter::planar_texture::optional_value(
			*(instances[0]->texture())));

	this->render(
		instances,
		instance_index(
			0u));

	planar_textures_[0u].set(
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
		shaders_[_instance.get()]);

	filter_manager_.quad().render(
		scoped_context.get());
}
