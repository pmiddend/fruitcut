#include "descriptor.hpp"
#include <fcppt/math/dim/comparison.hpp>

fruitcut::fruitlib::pp::texture::descriptor::descriptor(
	sge::renderer::dim2 const &_size,
	sge::image::color::format::type const _image_format,
	depth_stencil_format::type const _depth_stencil)
:
	tuple_(
		_size,
		_image_format,
		_depth_stencil)
{
}

sge::renderer::dim2 const &
fruitcut::fruitlib::pp::texture::descriptor::size() const
{
	return 
		boost::fusion::get<0>(
			tuple_);
}

sge::image::color::format::type 
fruitcut::fruitlib::pp::texture::descriptor::image_format() const
{
	return 
		boost::fusion::get<1>(
			tuple_);
}

fruitcut::fruitlib::pp::texture::depth_stencil_format::type 
fruitcut::fruitlib::pp::texture::descriptor::depth_stencil() const
{
	return 
		boost::fusion::get<2>(
			tuple_);
}

bool 
fruitcut::fruitlib::pp::texture::descriptor::operator<(
	descriptor const &r) const
{
	return 
		tuple_ < r.tuple_;
}
