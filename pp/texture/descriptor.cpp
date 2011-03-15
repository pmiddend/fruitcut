#include "descriptor.hpp"
#include <fcppt/math/dim/comparison.hpp>

fruitcut::pp::texture::descriptor::descriptor(
	sge::renderer::dim2 const &_size,
	sge::image::color::format::type const _image_format,
	sge::renderer::texture::filter::object const &_filter,
	depth_stencil_format::type const _depth_stencil)
:
	tuple_(
		_size,
		_image_format,
		filter_wrapper(
			_filter),
		_depth_stencil)
{
}

sge::renderer::dim2 const &
fruitcut::pp::texture::descriptor::size() const
{
	return 
		std::tr1::get<0>(
			tuple_);
}

sge::image::color::format::type 
fruitcut::pp::texture::descriptor::image_format() const
{
	return 
		std::tr1::get<1>(
			tuple_);
}

sge::renderer::texture::filter::object const &
fruitcut::pp::texture::descriptor::filter() const
{
	return 
		std::tr1::get<2>(
			tuple_).value();
}

fruitcut::pp::texture::depth_stencil_format::type 
fruitcut::pp::texture::descriptor::depth_stencil() const
{
	return 
		std::tr1::get<3>(
			tuple_);
}

bool 
fruitcut::pp::texture::descriptor::operator<(
	descriptor const &r) const
{
	return 
		tuple_ < r.tuple_;
}
