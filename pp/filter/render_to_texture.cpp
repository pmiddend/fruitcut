#include "render_to_texture.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/no_depth_stencil_texture.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/resource_flags_none.hpp>

fruitcut::pp::filter::render_to_texture::render_to_texture(
	sge::renderer::device_ptr const _renderer,
	sge::renderer::dim2 const &d,
	callback const &_callback)
:
	renderer_(
		_renderer),
	texture_(
		renderer_->create_texture(
			d,
			sge::image::color::format::rgb8,
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none)),
	target_(
		renderer_->create_target(
			texture_,
			sge::renderer::no_depth_stencil_texture())),
	callback_(
		_callback)
{
}

sge::renderer::texture_ptr const
fruitcut::pp::filter::render_to_texture::apply()
{
	sge::renderer::scoped_target scoped_target(
		renderer_,
		target_);

	sge::renderer::scoped_block scoped_block(
		renderer_);

	callback_();

	return texture_;
}

fruitcut::pp::filter::render_to_texture::~render_to_texture()
{
}
