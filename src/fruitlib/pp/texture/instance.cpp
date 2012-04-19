#include <fruitlib/pp/texture/instance.hpp>
#include <sge/renderer/target.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <fcppt/move.hpp>


fruitlib::pp::texture::instance::instance(
	fruitlib::pp::texture::descriptor const &_descriptor,
	sge::renderer::texture::planar_unique_ptr _texture,
	sge::renderer::target_unique_ptr _target,
	bool const _locked)
:
	descriptor_(
		_descriptor),
	texture_(
		fcppt::move(
			_texture)),
	target_(
		fcppt::move(
			_target)),
	locked_(
		_locked)
{
}

sge::renderer::texture::planar_shared_ptr
fruitlib::pp::texture::instance::texture()
{
	return
		texture_;
}

sge::renderer::target &
fruitlib::pp::texture::instance::target()
{
	return *target_;
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
