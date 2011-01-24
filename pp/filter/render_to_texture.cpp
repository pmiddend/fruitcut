#include "render_to_texture.hpp"
#include "../texture/manager.hpp"
#include "../texture/descriptor.hpp"
#include "../texture/instance.hpp"
#include <sge/renderer/device.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <iostream>

fruitcut::pp::filter::render_to_texture::render_to_texture(
	sge::renderer::device_ptr const _renderer,
	texture::manager &_texture_manager,
	sge::renderer::dim2 const &_texture_size,
	callback const &_callback)
:
	renderer_(
		_renderer),
	texture_manager_(
		_texture_manager),
	texture_size_(
		_texture_size),
	callback_(
		_callback)
{
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::filter::render_to_texture::apply()
{
	texture::counted_instance const result = 
		texture_manager_.query(
			texture::descriptor(
				texture_size_,
				sge::image::color::format::rgb8,
				sge::renderer::filter::linear));

	sge::renderer::scoped_target scoped_target(
		renderer_,
		result->target());

	sge::renderer::scoped_block scoped_block(
		renderer_);

	callback_();

	return result;
}

fruitcut::pp::filter::render_to_texture::~render_to_texture()
{
}
