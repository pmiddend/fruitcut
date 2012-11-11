#include <fruitlib/pp/filter/inject_texture.hpp>
#include <fruitlib/pp/texture/manager.hpp>
#include <fcppt/make_unique_ptr.hpp>


fruitlib::pp::filter::inject_texture::inject_texture(
	fruitlib::pp::texture::manager &_texture_manager)
:
	texture_manager_(
		_texture_manager),
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
		/*
		texture_manager_.query(
			*texture_);
			*/
		texture_counted_;
}

fruitlib::pp::filter::inject_texture::~inject_texture()
{
}
