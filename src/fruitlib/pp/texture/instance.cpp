#include "instance.hpp"

fruitcut::fruitlib::pp::texture::instance::instance(
	fruitcut::fruitlib::pp::texture::descriptor const &_descriptor,
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
fruitcut::fruitlib::pp::texture::instance::texture()
{
	return texture_;
}

sge::renderer::target_ptr const
fruitcut::fruitlib::pp::texture::instance::target()
{
	return target_;
}

bool 
fruitcut::fruitlib::pp::texture::instance::locked() const
{
	return locked_;
}

void
fruitcut::fruitlib::pp::texture::instance::locked(
	bool const _locked)
{
	locked_ = _locked;
}

fruitcut::fruitlib::pp::texture::descriptor const &
fruitcut::fruitlib::pp::texture::instance::descriptor() const
{
	return descriptor_;
}

fruitcut::fruitlib::pp::texture::instance::~instance()
{
}
