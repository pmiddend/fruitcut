#include <fruitlib/pp/texture/instance.hpp>


fruitlib::pp::texture::instance::instance(
	fruitlib::pp::texture::descriptor const &_descriptor,
	sge::renderer::texture::planar_ptr const _texture,
	sge::renderer::target_ptr const _target,
	bool const _locked)
:
	descriptor_(
		_descriptor),
	texture_(
		_texture),
	target_(
		_target),
	locked_(
		_locked)
{
}

sge::renderer::texture::planar_ptr const
fruitlib::pp::texture::instance::texture()
{
	return texture_;
}

sge::renderer::target_ptr const
fruitlib::pp::texture::instance::target()
{
	return target_;
}

bool
fruitlib::pp::texture::instance::locked() const
{
	return locked_;
}

void
fruitlib::pp::texture::instance::locked(
	bool const _locked)
{
	locked_ = _locked;
}

fruitlib::pp::texture::descriptor const &
fruitlib::pp::texture::instance::descriptor() const
{
	return descriptor_;
}

fruitlib::pp::texture::instance::~instance()
{
}
