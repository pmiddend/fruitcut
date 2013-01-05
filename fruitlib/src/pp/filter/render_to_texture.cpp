#include <fruitlib/pp/filter/manager.hpp>
#include <fruitlib/pp/filter/render_to_texture.hpp>
#include <fruitlib/pp/texture/descriptor.hpp>
#include <fruitlib/pp/texture/instance.hpp>
#include <fruitlib/pp/texture/manager.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/context/scoped_core.hpp>
#include <sge/renderer/target/offscreen.hpp>


fruitlib::pp::filter::render_to_texture::render_to_texture(
	fruitlib::pp::filter::manager &_filter_manager,
	fruitlib::pp::texture::manager &_texture_manager,
	fruitlib::pp::filter::texture_size const &_texture_size,
	sge::renderer::clear::parameters const &_clear_parameters,
	callback const &_callback,
	fruitlib::pp::texture::depth_stencil_format::type const _depth_stencil)
:
	filter_manager_(
		_filter_manager),
	texture_manager_(
		_texture_manager),
	texture_size_(
		_texture_size),
	clear_parameters_(
		_clear_parameters),
	callback_(
		_callback),
	depth_stencil_(
		_depth_stencil)
{
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::filter::render_to_texture::apply()
{
	fruitlib::pp::texture::counted_instance const result =
		texture_manager_.query(
			fruitlib::pp::texture::descriptor(
				texture_size_.get(),
				sge::image::color::format::rgb8,
				depth_stencil_));

	result->target().clear(
		clear_parameters_);

	sge::renderer::context::scoped_core scoped_context(
		filter_manager_.renderer(),
		result->target());

	callback_(
		scoped_context.get());

	return result;
}

fruitlib::pp::filter::render_to_texture::~render_to_texture()
{
}
