#include "instance.hpp"

fruitcut::pp::texture::instance::instance(
	fruitcut::pp::texture::descriptor const &_descriptor,
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
fruitcut::pp::texture::instance::texture()
{
	return texture_;
}

sge::renderer::target_ptr const
fruitcut::pp::texture::instance::target()
{
	return target_;
}

bool 
fruitcut::pp::texture::instance::locked() const
{
	return locked_;
}

void
fruitcut::pp::texture::instance::locked(
	bool const _locked)
{
	locked_ = _locked;
}

fruitcut::pp::texture::descriptor const &
fruitcut::pp::texture::instance::descriptor() const
{
	return descriptor_;
}

fruitcut::pp::texture::instance::~instance()
{
}
