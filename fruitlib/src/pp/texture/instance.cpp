#include <fruitlib/pp/texture/instance.hpp>
#include <fruitlib/pp/texture/optional_depth_stencil_surface.hpp>
#include <sge/renderer/depth_stencil_buffer/surface.hpp>
#include <sge/renderer/target/offscreen.hpp>
#include <sge/renderer/texture/depth_stencil.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <fcppt/config/external_begin.hpp>
#include <utility>
#include <fcppt/config/external_end.hpp>


fruitlib::pp::texture::instance::instance(
	fruitlib::pp::texture::descriptor const &_descriptor,
	sge::renderer::texture::planar_unique_ptr _texture,
	sge::renderer::target::offscreen_unique_ptr _target,
	fruitlib::pp::texture::optional_depth_stencil_surface _depth_stencil,
	fruitlib::pp::texture::is_locked const &_locked)
:
	descriptor_(
		_descriptor),
	texture_(
		std::move(
			_texture)),
	target_(
		std::move(
			_target)),
	depth_stencil_(
		std::move(
			_depth_stencil)),
	locked_(
		_locked.get())
{
}

sge::renderer::texture::planar_shared_ptr
fruitlib::pp::texture::instance::texture()
{
	return
		texture_;
}

sge::renderer::target::offscreen &
fruitlib::pp::texture::instance::target()
{
	return
		*target_;
}

bool
fruitlib::pp::texture::instance::locked() const
{
	return
		locked_;
}

void
fruitlib::pp::texture::instance::locked(
	bool const _locked)
{
	locked_ =
		_locked;
}

fruitlib::pp::texture::descriptor const &
fruitlib::pp::texture::instance::descriptor() const
{
	return
		descriptor_;
}

fruitlib::pp::texture::instance::~instance()
{
}
