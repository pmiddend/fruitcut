#include <fruitlib/pp/filter/inject_texture.hpp>
#include <fruitlib/pp/texture/manager.hpp>


fruitlib::pp::filter::inject_texture::inject_texture(
	texture::manager &_texture_manager)
:
	texture_manager_(
		_texture_manager),
	texture_()
{
}

void
fruitlib::pp::filter::inject_texture::texture(
	sge::renderer::texture::planar_shared_ptr const _texture)
{
	texture_ =
		_texture;
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::filter::inject_texture::apply()
{
	return
		texture_manager_.query(
			*texture_);
}

fruitlib::pp::filter::inject_texture::~inject_texture()
{
}
