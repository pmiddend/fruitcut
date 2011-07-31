#include "render_to_texture.hpp"
#include "../texture/manager.hpp"
#include "../texture/descriptor.hpp"
#include "../texture/instance.hpp"
#include <sge/renderer/device.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <fcppt/io/cout.hpp>
#include <iostream>

fruitlib::pp::filter::render_to_texture::render_to_texture(
	sge::renderer::device &_renderer,
	texture::manager &_texture_manager,
	sge::renderer::dim2 const &_texture_size,
	callback const &_callback,
	texture::depth_stencil_format::type const _depth_stencil)
:
	renderer_(
		_renderer),
	texture_manager_(
		_texture_manager),
	texture_size_(
		_texture_size),
	callback_(
		_callback),
	depth_stencil_(
		_depth_stencil)
{
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::filter::render_to_texture::apply()
{
	//std::cerr << "rtt::apply\n";
	texture::counted_instance const result = 
		texture_manager_.query(
			texture::descriptor(
				texture_size_,
				sge::image::color::format::rgb8,
				depth_stencil_));

	sge::renderer::scoped_target scoped_target(
		renderer_,
		*result->target());

	sge::renderer::scoped_block scoped_block(
		renderer_);

	callback_();

	return result;
}

fruitlib::pp::filter::render_to_texture::~render_to_texture()
{
}
