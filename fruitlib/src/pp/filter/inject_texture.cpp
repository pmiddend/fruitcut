#include <fruitlib/pp/filter/inject_texture.hpp>


fruitlib::pp::filter::inject_texture::inject_texture()
:
	texture_counted_()
{
}

void
fruitlib::pp::filter::inject_texture::texture(
	fruitlib::pp::texture::counted_instance const &_texture_counted)
{
	texture_counted_ =
		_texture_counted;
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::filter::inject_texture::apply()
{
	return
		texture_counted_;
}

fruitlib::pp::filter::inject_texture::~inject_texture()
{
}
