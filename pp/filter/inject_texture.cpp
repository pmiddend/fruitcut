#include "inject_texture.hpp"
#include "../texture/manager.hpp"

fruitcut::pp::filter::inject_texture::inject_texture(
	texture::manager &_texture_manager)
:
	texture_manager_(
		_texture_manager),
	texture_()
{
}

void
fruitcut::pp::filter::inject_texture::texture(
	sge::renderer::texture_ptr const _texture)
{
	texture_ = 
		_texture;
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::filter::inject_texture::apply()
{
	return 
		texture_manager_.query(
			texture_);
}

fruitcut::pp::filter::inject_texture::~inject_texture()
{
}
